global ft_strdup
extern malloc
extern ft_strcpy
extern ft_strlen

section .text

ft_strdup:
    mov     rax, 0
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
    mov rax, 0
    ret
