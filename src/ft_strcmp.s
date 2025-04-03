global ft_strcmp

ft_strcmp:
    mov rax, 0
    mov r8b, byte [rdi]
    cmp r8b, 0
    je  done
    mov r9b, byte [rsi]
    cmp r9b, 0
    je  done
    cmp r8b, r9b
    jne done
    inc rdi
    inc rsi
    jmp ft_strcmp

done:
    mov     r8b, [rdi]
    mov     r9b, [rsi]
    sub     r9b, r8b
    movzx   rax, r9b
    ret

