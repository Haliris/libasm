global ft_list_sort
extern ft_list_size

struc Node
    .data resq 1
    .next resq 1
endstruc

section .text
ft_list_sort:
    push    rbp
    mov     rbp, rsp
    push    r12
    push    r13
    push    r14
    push    r15
    push    rbx

    test    rdi, rdi
    jz      done
    test    rsi, rsi
    jz      done

    mov     r13, rdi
    mov     r12, rsi
    mov     rdi, [r13]
    test    rdi, rdi
    jz      done

    mov     rax, [rdi + Node.next]
    test    rax, rax
    jz      done

    call    ft_list_size
    test    rax, rax
    jz      done
    mov     r15, rax

compare_outer_loop:
    test    r15, r15
    jz      done
    mov     r14, [r13]
    mov     rbx, r15
    dec     rbx

compare_inner_loop:
    test    rbx, rbx
    jz      next_node

    mov     rdi, [r14 + Node.data]
    mov     rsi, [r14 + Node.next]
    test    rsi, rsi
    jz      next_node

    push    r14
    push    rbx
    push    r15

    call    r12

    pop     r15
    pop     rbx
    pop     r14

    cmp     rax, 0
    jle     no_swap
    mov     rsi, [r14 + Node.next]
    mov     rax, [r14 + Node.data]
    mov     rdx, [rsi + Node.data]
    mov     [r14 + Node.data], rdx
    mov     [rsi + Node.data], rax

no_swap:
    mov r14, [r14 + Node.next]
    dec rbx
    jmp compare_inner_loop
    
next_node:
    dec     r15
    jmp     compare_outer_loop

done:
    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    ret

