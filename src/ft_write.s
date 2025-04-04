global ft_write
extern __errno_location

ft_write:
    mov     rax, 1
    syscall
    test    rax, rax
    js      error
    ret

error:
    neg     rax
    mov     rdi, rax
    call    __errno_location wrt ..plt
    mov     [rax], rdi
    mov     rax, -1
    ret
