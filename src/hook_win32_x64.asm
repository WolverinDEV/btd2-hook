extern hook_c_callback
global asm_hook_execute
; Code which will be injected
; CALL #<Hook Config>

; Hook Config
; PUSH rax
; MOV rax, $0xFFFFFFFEFFFFFFFF # Target address
; CALL asm__hook_execute
; <Execute overridden>
; RET

section .text
; When juming here
; rax will contain the hook config.
asm_hook_execute:
; PUSH rax Has already been done
; PUSH rcx Has already been done
PUSH rdx
PUSH rbx
PUSH rsi
PUSH rdi
PUSH rsp
PUSH rbp
PUSH r8
PUSH r9
PUSH r10
PUSH r11
PUSH r12
PUSH r13
PUSH r14
PUSH r15
PUSHFQ

; RCX Contains the hook config
MOV rdx, rsp ; Second argument the pointer to the registers

MOV rbx, rsp
SUB rsp, 0x20 ; 0x20 Shadow stack
AND rsp, -16 ; Align the stack ptr
CALL hook_c_callback
MOV rsp, rbx  ; Restore stack ptr

POPFQ
POP r15
POP r14
POP r13
POP r12
POP r11
POP r10
POP r9
POP r8
POP rbp
POP rsp
POP rdi
POP rsi
POP rbx
POP rdx
; POP rcx Must be restored later
; POP rax Must be restored later

ret