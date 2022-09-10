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

            void* buffer_original{nullptr};

            [[nodiscard]] auto create_buffer_config() -> bool;
            [[nodiscard]] auto inject_jump() -> bool;
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

auto JumpHook::inject_jump() -> bool {
    if(this->override_length < 13) {
        /* Not possible. */
        return false;
    }

    if(this->buffer_original) {
        logging::warn("inject_jump() has already been called!");
        return true;
    }

    this->buffer_original = malloc(this->override_length);
    mem::vm_read(this->address, this->buffer_original, this->override_length);

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

#ifdef HOOK_DEBUG
    logging::debug("Hook Config Addr: 0x{:X}", (uintptr_t) this->buffer_config);
    logging::debug("Override {:#x} {} bytes", this->address, call_asm.size());
    logging::debug("{}", logging::hex_dump(call_asm.data(), call_asm.size()));
#endif
    return mem::vm_write(this->address, call_asm.data(), call_asm.size());
}

// The idea is simple.
// 1. Load the original RAX (has been overridden in order to jump here)
// 2. Save RAX twice (use the first RAX as jump back address)
// 3. Store the return address on the stack (before the second RAX save)
// 4. Push RCX and store the config ptr
// 5. Call the fixed assembly code which pops RCX, RAX
auto JumpHook::create_buffer_config() -> bool {
    this->buffer_config_size = this->override_length + 0x100;
    auto buffer = (uint8_t*) VirtualAlloc(nullptr, this->buffer_config_size, MEM_COMMIT, PAGE_READWRITE);
    if(!buffer) {
        logging::error("Failed to allocate hook config buffer ({} bytes).", this->buffer_config_size);
        return false;
    }

    size_t idx{0};

    // push   rcx
    *(uint8_t*)   (buffer + idx++) = 0x51;

    // movabs rcx, <config address>
    *(uint8_t*)   (buffer + idx++) = 0x48;
    *(uint8_t*)   (buffer + idx++) = 0xB9;
    *(uintptr_t*) (buffer + idx  ) = (uintptr_t) this; idx += 8;

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
    mem::vm_read(this->address, buffer + idx, this->override_length); idx += this->override_length;
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
        *(uintptr_t*) (buffer + idx ) = this->address + this->override_length; idx += 8;

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

    DWORD dummy;
    VirtualProtect(buffer, idx, PAGE_EXECUTE_READ, &dummy);

    this->buffer_config = buffer;
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
    //logging::info("Having hook callback with 0x{:X} 0x{:X}", (uintptr_t) hook, (uintptr_t) registers);
    hook->callback(registers);
}