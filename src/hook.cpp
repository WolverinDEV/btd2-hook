#include "./hook.h"
#include "./log.h"
#include "./mem.h"

#include <cstdint>
#include <array>
#include <vector>

#include <Windows.h>

// Code which will be injected
// CALL #<Hook Config>

// Hook Config
// PUSH rax
// MOV rax, $0xFFFFFFFEFFFFFFFF # Target address
// CALL asm__hook_execute
// <Execute overridden>
// RET

#define HOOK_DEBUG

extern "C" uintptr_t asm_hook_execute;

using hook::JumpHook;

namespace hook {
    class JumpHook {
        public:
            explicit JumpHook(uintptr_t address, size_t override_length, hook_callback_t&& callback)
                : address{address}, override_length{override_length}, callback{std::forward<hook_callback_t&&>(callback)}
            { }

            virtual ~JumpHook();

            [[nodiscard]] auto initialize() -> bool;

            hook_callback_t callback;
        private:
            uintptr_t address{};
            size_t override_length{};

            void* buffer_config{nullptr};
            size_t buffer_config_size{0};
            bool buffer_config_abs_jmp{false};

            void* buffer_original{nullptr};

            [[nodiscard]] auto create_buffer_config() -> bool;
            [[nodiscard]] auto inject_jump() -> bool;

            [[nodiscard]] auto inject_jump_rel() -> bool;
            [[nodiscard]] auto inject_jump_abs() -> bool;
    };
}

JumpHook::~JumpHook() {
    logging::debug("JumpHook::~JumpHook at 0x{:X}", this->address);
    auto buffer_original = std::exchange(this->buffer_original, nullptr);
    if(buffer_original) {
        mem::vm_write(this->address, buffer_original, this->override_length);
        free(buffer_original);
    }

    auto buffer_config = std::exchange(this->buffer_config, nullptr);
    auto buffer_config_size = std::exchange(this->buffer_config_size, 0);
    if(buffer_config) {
        VirtualFree(buffer_config, buffer_config_size, MEM_RELEASE);
    }
}

auto JumpHook::initialize() -> bool {
    if(!this->create_buffer_config()) {
        return false;
    }

    return this->inject_jump();
}

void* allocate_nearby(uintptr_t target, size_t size)
{
    MEMORY_BASIC_INFORMATION mbi{};
    auto handle_process = GetCurrentProcess();
    auto begin = target - 0x7FF00000;
    auto end = target + 0x7FF00000;
    for (auto curr = begin; curr < end; curr += mbi.RegionSize) {
        if(!VirtualQueryEx(handle_process, (void*) curr, &mbi, sizeof(mbi))) {
            continue;
        }

        if(mbi.State != MEM_FREE) {
            continue;
        }

        auto memory = VirtualAllocEx(handle_process, mbi.BaseAddress, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if(!memory) {
            continue;
        }

        return memory;
    }

    logging::warn("Failed to allocate memory near 0x{:X}. Using default allocation.", target);
    return VirtualAlloc(
        nullptr,
        size,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );
}

[[nodiscard]]
bool is_reljump_supported(uintptr_t jmp_source, uintptr_t jmp_target) {
    uintptr_t abs_offset = jmp_source > jmp_target ?
                           jmp_source - jmp_target :
                           jmp_target - jmp_source;

    return abs_offset > 0x7FFFFFFF;
}

auto JumpHook::inject_jump() -> bool {
    if(!this->buffer_config) {
        logging::error("Could not inject jump when we don't have a target.");
        return false;
    }

    /* Save the original instructions. */
    {
        if(this->buffer_original) {
            logging::warn("inject_jump() has already been called!");
            return true;
        }

        this->buffer_original = malloc(this->override_length);
        mem::vm_read(this->address, this->buffer_original, this->override_length);
    }

    if(!this->buffer_config_abs_jmp) {
        /* we can juse a relative jump. */
        if(!this->inject_jump_rel()) {
            return false;
        }

#ifdef HOOK_DEBUG
        logging::debug("Created trampoline at 0x{:X} -> 0x{:X} (rel jmp)", this->address, (uintptr_t) this->buffer_config);
#endif
    } else {
        if(!this->inject_jump_abs()) {
            return false;
        }

#ifdef HOOK_DEBUG
        logging::debug("Created trampoline at 0x{:X} -> 0x{:X} (abs jmp)", this->address, (uintptr_t) this->buffer_config);
#endif
    }

    return true;
}

auto JumpHook::inject_jump_rel() -> bool {
    if(this->override_length < 6) {
        /* Not possible. */
        return false;
    }

    std::vector<uint8_t> call_asm{};
    call_asm.resize(this->override_length, 0x90); // NOP

    auto offset = (intptr_t) this->buffer_config - (intptr_t) this->address - 5;
    if(offset < -0x7FFFFFFF || offset > 0x7FFFFFFF) {
        /* Offset out of range. */
        return false;
    }

    // JMP rel32
    call_asm[0] = 0xE9;
    *(int32_t*) (&call_asm[1]) = (int32_t) offset;

    return mem::vm_write(this->address, call_asm.data(), call_asm.size());
}

auto JumpHook::inject_jump_abs() -> bool {
    if(this->override_length < 13) {
        /* Not possible. */
        return false;
    }

    std::vector<uint8_t> call_asm{};
    call_asm.resize(this->override_length, 0x90); // NOP

    // push rax
    call_asm[0] = 0x50;

    // mov rax, QWORD
    call_asm[1] = 0x48;
    call_asm[2] = 0xB8;
    *(uintptr_t*) &call_asm[3] = (uintptr_t) this->buffer_config;

    // jmp rax
    call_asm[11] = 0xFF;
    call_asm[12] = 0xE0;

    return mem::vm_write(this->address, call_asm.data(), call_asm.size());
}

// The idea is simple.
// 1. Load the original RAX (has been overridden in order to jump here)
// 2. Save RAX twice (use the first RAX as jump back address)
// 3. Store the return address on the stack (before the second RAX save)
// 4. Push RCX and store the config ptr
// 5. Call the fixed assembly code which pops RCX, RAX
void write_buffer_config(uint8_t* buffer, uintptr_t jmp_source, size_t override_length, void* callback_arg, bool is_abs_jmp) {
    size_t idx{0};

    // If it's not a abs jump we need to push the rax register.
    if(!is_abs_jmp) {
        // push   rax
        *(uint8_t*)   (buffer + idx++) = 0x50;
    }

    // push   rcx
    *(uint8_t*)   (buffer + idx++) = 0x51;

    // movabs rcx, <config address>
    *(uint8_t*)   (buffer + idx++) = 0x48;
    *(uint8_t*)   (buffer + idx++) = 0xB9;
    *(uintptr_t*) (buffer + idx  ) = (uintptr_t) callback_arg; idx += 8;

    // movabs rax, <target address> (Has been saved when jumping here)
    *(uint8_t*)   (buffer + idx++) = 0x48;
    *(uint8_t*)   (buffer + idx++) = 0xB8;
    *(uintptr_t*) (buffer + idx ) = (uintptr_t) &asm_hook_execute; idx += 8;

    // call rax
    *(uint8_t*)   (buffer + idx++) = 0xFF;
    *(uint8_t*)   (buffer + idx++) = 0xD0;

    // pop    rcx
    *(uint8_t*)   (buffer + idx++) = 0x59;

    // pop    rax
    *(uint8_t*)   (buffer + idx++) = 0x58;

    // Overridden assembly codes (surrounded by a single nop)
    *(uint8_t*)   (buffer + idx++) = 0x90; // nop
    mem::vm_read(jmp_source, buffer + idx, override_length); idx += override_length;
    *(uint8_t*)   (buffer + idx++) = 0x90; // nop

    // Push the return address onto the stack without modifying any registers....
    {
        // push rax
        *(uint8_t*)   (buffer + idx++) = 0x50;

        // push rax
        *(uint8_t*)   (buffer + idx++) = 0x50;

        // movabs rax, <return address>
        *(uint8_t*)   (buffer + idx++) = 0x48;
        *(uint8_t*)   (buffer + idx++) = 0xB8;
        *(uintptr_t*) (buffer + idx ) = jmp_source + override_length; idx += 8;

        // mov QWORD PTR [rsp+0x8],rax
        *(uint8_t*)   (buffer + idx++) = 0x48;
        *(uint8_t*)   (buffer + idx++) = 0x89;
        *(uint8_t*)   (buffer + idx++) = 0x44;
        *(uint8_t*)   (buffer + idx++) = 0x24;
        *(uint8_t*)   (buffer + idx++) = 0x08;

        // pop    rax
        *(uint8_t*)   (buffer + idx++) = 0x58;

        // ret
        *(uint8_t*)   (buffer + idx++) = 0xC3;
    }
}


auto JumpHook::create_buffer_config() -> bool {
    this->buffer_config_size = this->override_length + 0x100;
    this->buffer_config = allocate_nearby(this->address, this->buffer_config_size);
    if(!this->buffer_config) {
        logging::error("Failed to allocate hook config buffer ({} bytes).", this->buffer_config_size);
        return false;
    }

    this->buffer_config_abs_jmp = is_reljump_supported(this->address, (uintptr_t) this->buffer_config);
    write_buffer_config((uint8_t*) this->buffer_config, this->address, this->override_length, this, this->buffer_config_abs_jmp);
    return true;
}

std::shared_ptr<hook::JumpHook> hook::jump(uintptr_t address, size_t override_length, hook_callback_t callback) {
    if(override_length < 5) {
        /* We need at least 5 bytes for the call. */
        return nullptr;
    }

    auto hook_instance = std::make_shared<hook::JumpHook>(address, override_length, std::forward<hook_callback_t>(callback));
    if(!hook_instance->initialize()) {
        return nullptr;
    }
    return hook_instance;
}

extern "C" [[maybe_unused]] void hook_c_callback(JumpHook* hook, hook::Registers* registers) {
    // logging::info("Having hook callback with 0x{:X} 0x{:X}", (uintptr_t) hook, (uintptr_t) registers);
    hook->callback(registers);
}