global ft_strcpy

ft_strcpy:
    mov rax, 0
    jmp cpy_loop

cpy_loop:
    cmp byte [rsi + rax], 0
    je done
    mov dl, [rsi + rax]
    mov [rdi + rax], dl
    inc rax
    jmp cpy_loop

done:
    mov byte [rdi + rax], 0
    mov rax, rdi
    ret
