.text

        .global _start

_start:
        /* Call hello_world 5 times */
        movq    $5, %rdi
        movq    $hello_world, %rsi
        call    multiple

        movq    $60, %rax
        xor     %rdi, %rdi
        syscall

hello_world:
        pushq   %rbp
        movq    %rsp, %rbp
        movq    $1, %rax
        movq    $1, %rdi
        movq    $msg, %rsi
        movq    $len, %rdx
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
multi_loop:
        test    %rbx,%rbx
        je      multi_end
        call    *%r12
        dec     %rbx
        jmp     multi_loop
multi_end:
        popq    %rbx
        popq    %r12
        popq    %rbp
        retq

.data

msg:
        .ascii "Hello, world!\n"
        len =  . - msg
