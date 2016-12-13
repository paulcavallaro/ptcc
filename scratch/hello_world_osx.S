.extern _printf

.text

.global _main
.global strlen_sse2
.global strlen_sse4

_main:
        /* 16 byte align stack pointer */
        sub     $0x8, %rsp

        /* Call hello_world 5 times */
        movq    $5, %rdi
        movq    hello_world@GOTPCREL(%rip), %rsi
        call    multiple

        /* Get length of a string w/ strlen_sse2 */
        movq    other_msg@GOTPCREL(%rip), %rdi
        call    strlen_sse2
        /* Print out return value in %rax */
        movq    fmt_str@GOTPCREL(%rip), %rdi
        movq    %rax, %rsi
        call    *_printf@GOTPCREL(%rip)

        /* Get length of a string w/ strlen_sse4 */
        movq    other_msg@GOTPCREL(%rip), %rdi
        call    strlen_sse4
        /* Print out return value in %rax */
        movq    fmt_str@GOTPCREL(%rip), %rdi
        movq    %rax, %rsi
        call    *_printf@GOTPCREL(%rip)

        /* OSX adds 0x2000000 to syscall numbers */
        /* Call sys exit */
        movq    $0x2000001, %rax
        xor     %rdi, %rdi
        syscall

hello_world:
        pushq   %rbp
        movq    %rsp, %rbp
        movq    $0x2000004, %rax
        movq    $1, %rdi
        movq    msg@GOTPCREL(%rip), %rsi
        movq    $14, %rdx
        syscall
        popq    %rbp
        retq

multiple:
        pushq   %rbp
        mov     %rsp, %rbp
        /* Use Callee-saved registers rbx + r12 */
        pushq   %rbx
        pushq   %r12
        mov     %rdi, %rbx
        mov     %rsi, %r12
.multi_loop:
        test    %rbx,%rbx
        je      .multi_end
        call    *%r12
        dec     %rbx
        jmp     .multi_loop
.multi_end:
        popq    %rbx
        popq    %r12
        popq    %rbp
        retq

strlen_sse2:
        mov             $-16, %rax
        pxor            %xmm0, %xmm0
.strlen_sse2.loop:
        addq            $16, %rax
        movdqu          (%rdi,%rax), %xmm1
        pcmpeqb         %xmm0, %xmm1
        pmovmskb        %xmm1, %ecx
        test            %ecx, %ecx
        jz              .strlen_sse2.loop

        bsf             %ecx, %ecx
        addq            %rcx, %rax
        retq

strlen_sse4:

        movq            $-16, %rax
        pxor            %xmm0, %xmm0

.strlen_sse4.loop:      /* Must perform addition before the string comparison */
                        /* doing it after would interfere with the status flags */

        addq            $16, %rax
        pcmpistri       $0x08, (%rdi,%rax,1), %xmm0       /* EQUAL_EACH */
        jnz             .strlen_sse4.loop

        add             %rcx, %rax
        retq

.data

fmt_str:
        .asciz "rax = %lld\n"
msg:
        .ascii "Hello, world!\n"
        len =  . - msg
other_msg:
        .asciz "This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. See a"
