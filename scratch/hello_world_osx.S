.text

        .global _start

_start:
        /* Call hello_world 5 times */
        movq    $5, %rdi
        /* OSX forces use of rip relative addressing via global offset table */
        movq    hello_world@GOTPCREL(%rip), %rsi
        call    multiple

        /* OSX adds 0x2000000 to syscall numbers */
        movq    $0x2000001, %rax
        xor     %rdi, %rdi
        syscall

hello_world:
        pushq   %rbp
        movq    %rsp, %rbp
        /* OSX adds 0x2000000 to syscall numbers */
        movq    $0x2000004, %rax
        movq    $1, %rdi
        movq    msg@GOTPCREL(%rip), %rsi
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
