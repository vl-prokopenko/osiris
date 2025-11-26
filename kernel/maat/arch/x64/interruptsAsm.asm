extern handle

%macro ISR_NOERR 2
global do_isr%1
do_isr%1:
    cli
    push 0
    push %2
    jmp isr_common_stub
%endmacro

%macro ISR_ERR 2
global do_isr%1
do_isr%1:
    cli
    push %2
    jmp isr_common_stub
%endmacro

isr_common_stub:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp
    call handle

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16
    iretq

ISR_NOERR 0, 0
ISR_NOERR 1, 1
ISR_NOERR 2, 2
ISR_NOERR 3, 3
ISR_NOERR 4, 4
ISR_NOERR 5, 5
ISR_NOERR 6, 6
ISR_NOERR 7, 7
ISR_ERR   8, 8
ISR_NOERR 9, 9
ISR_ERR   10, 10
ISR_ERR   11, 11
ISR_ERR   12, 12
ISR_ERR   13, 13 
ISR_ERR   14, 14
ISR_NOERR 15, 15
ISR_NOERR 16, 16
ISR_ERR   17, 17
ISR_NOERR 18, 18

ISR_NOERR 32, 32

; IRQ 0, 32
; IRQ 1, 33