.intel_syntax noprefix

.extern _printf

.text

.global _main
.global strlen_sse2
.global strlen_sse4

_main:
        /* 16 byte align stack pointer */
        sub     rsp, 0x8

        /* Call hello_world 5 times */
        mov     rdi, 5
        mov     rsi, [rip+hello_world@GOTPCREL]
        call    multiple

        /* Get length of a string w/ strlen_sse2 */
        mov     rdi, [rip+other_msg@GOTPCREL]
        call    strlen_sse2
        /* Print out return value in rax */
        mov     rdi, [rip+fmt_str@GOTPCREL]
        mov     rsi, rax
        call    [rip+_printf@GOTPCREL]

        /* Get length of a string w/ strlen_sse4 */
        mov     rdi, [rip+other_msg@GOTPCREL]
        call    strlen_sse4
        /* Print out return value in rax */
        mov     rdi, [rip+fmt_str@GOTPCREL]
        mov     rsi, rax
        call    [rip+_printf@GOTPCREL]

        /* Call sys exit */
        mov     rax, 60
        xor     rdi, rdi
        syscall

hello_world:
        push    rbp
        mov     rbp, rsp
        mov     rax, 1
        mov     rdi, 1
        mov     rsi, [rip+msg@GOTPCREL]
        mov     rdx, 14
        syscall
        pop     rbp
        ret

multiple:
        push    rbp
        mov     rbp, rsp
        /* Use Callee-saved registers rbx + r12 */
        push    rbx
        push    r12
        mov     rbx, rdi
        mov     r12, rsi
.multi_loop:
        test    rbx,rbx
        je      .multi_end
        call    r12
        dec     rbx
        jmp     .multi_loop
.multi_end:
        pop     rbx
        pop     r12
        pop     rbp
        ret

strlen_sse2:
        mov             rax, -16
        pxor            xmm0, xmm0
.strlen_sse2.loop:
        add             rax, 16
        movdqu          xmm1, [rdi + rax]
        pcmpeqb         xmm1, xmm0
        pmovmskb        ecx, xmm1
        test            ecx, ecx
        jz              .strlen_sse2.loop

        bsf             ecx, ecx
        add             rax, rcx
        ret

strlen_sse4:

        mov             rax, -16
        pxor            xmm0, xmm0

.strlen_sse4.loop:      /* Must perform addition before the string comparison */
                        /* doing it after would interfere with the status flags */

        add             rax, 16
        pcmpistri       xmm0, [rdi + rax], 0x08         /* EQUAL_EACH */
        jnz             .strlen_sse4.loop

        add             rax, rcx
        ret

.data

fmt_str:
        .asciz "rax = %lld\n"
msg:
        .ascii "Hello, world!\n"
        len =  . - msg
other_msg:
        .asciz "This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. See a"
