global ft_strdup
extern malloc
extern ft_strcpy
extern ft_strlen
extern __errno_location

section .text

ft_strdup:
    call    ft_strlen wrt ..plt

    inc     rax
    push    rdi
    mov     rdi, rax

    call    malloc wrt ..plt
    test    rax, rax
    jz      error

    pop     rsi
    mov     rdi, rax
    call    ft_strcpy wrt ..plt
    ret

error:
    neg     rax
    mov     rdi, rax
    call    __errno_location wrt ..plt
    mov     [rax], rdi
    mov     rax, -1
    pop     rsi
    xor     rax, rax
    ret
