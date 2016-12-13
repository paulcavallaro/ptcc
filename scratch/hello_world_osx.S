.extern _printf

.text

.global _main

_main:
        /* Call hello_world 5 times */
        movq    $5, %rdi
        movq    hello_world@GOTPCREL(%rip), %rsi
        call    multiple

        /* Get length of a string */
        movq    other_msg@GOTPCREL(%rip), %rdi
        call    strlen_sse2
        /* Print out return value in %rax */
        /* 16 byte align stack pointer */
        sub     $0x8, %rsp
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
.loop:
        addq            $16, %rax
        movdqu          (%rdi,%rax), %xmm1
        pcmpeqb         %xmm0, %xmm1
        pmovmskb        %xmm1, %ecx
        test            %ecx, %ecx
        jz              .loop

        bsf             %ecx, %ecx
        addq            %rcx, %rax
        ret


.data

fmt_str:
        .asciz "rax = %lld\n"
msg:
        .ascii "Hello, world!\n"
        len =  . - msg
other_msg:
        .asciz "This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. This is a longer string that repeats itself. See a"
