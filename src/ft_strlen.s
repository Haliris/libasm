global ft_strlen

ft_strlen:
    mov rax, 0
    jmp count_loop

count_loop:
    cmp byte [rdi + rax], 0
    je  done
    inc rax
    jmp count_loop

done:
    ret

