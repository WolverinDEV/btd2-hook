#pragma once

#include <memory>
#include <functional>

namespace hook {
    class JumpHook;

#pragma pack(push, 1)
    struct Registers {
        uint64_t flags;

        uint64_t r15;
        uint64_t r14;
        uint64_t r13;
        uint64_t r12;
        uint64_t r11;
        uint64_t r10;
        uint64_t r9;
        uint64_t r8;
        uint64_t rbp;
        // Attention: Due to the hook this includes an offset of 0x??
        uint64_t rsp;
        uint64_t rdi;
        uint64_t rsi;
        uint64_t rbx;
        uint64_t rdx;

        uint64_t _reserved;

        uint64_t rcx;
        uint64_t rax;
    };
#pragma pack(pop)

    typedef std::function<void(Registers*)> hook_callback_t;

    // Inject a jump hook which needs at least 5 bytes of override length.
    // Attention: The overridden bytes must *not* contains a RET or RELCALL instruction!
    extern std::shared_ptr<JumpHook> jump(uintptr_t /* address */, size_t /* override length */, hook_callback_t /* callback */);
}