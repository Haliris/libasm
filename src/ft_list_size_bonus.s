global ft_list_size

struc Node
    .data resq 1
    .next resq 1
endstruc

section .text
ft_list_size:
    xor     rax, rax
    test    rdi, rdi
    jz      done
    jmp     list_loop

list_loop:
    test    rdi, rdi
    jz      done
    inc     rax
    mov     rdi, [rdi + Node.next]
    jmp     list_loop

done:
    ret
