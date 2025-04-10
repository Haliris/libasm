global ft_list_sort
extern ft_list_size
extern printf

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
    mov     rcx, [rdi + Node.next]
    test    rcx, rcx
    jz      done
    call    ft_list_size
    test    rax, rax
    jz      done
    mov     r15, rax
    jmp     compare_outer_loop

compare_outer_loop:
    test    r15, r15
    jz      done
    mov     r14, [r13]
    mov     rbx, r15
    dec     rbx

compare_inner_loop:
    test    rbx, rbx
    jz      next_node
    mov     rcx, [r14 + Node.next]
    test    rcx, rcx
    jz      next_node
    mov     rdi, [r14 + Node.data]
    mov     rsi, [rcx + Node.data]

    push    rcx
    push    r14
    push    rbx
    push    r15
    sub     rsp, 8
    call    r12
    add     rsp, 8
    pop     r15
    pop     rbx
    pop     r14
    pop     rcx
    cmp     rax, 0
    je      do_swap
    jmp     no_swap

do_swap:
    mov     rsi, [rcx + Node.data]
    mov     rdx, [r14 + Node.data]
    mov     [r14 + Node.data], rsi
    mov     [rcx + Node.data], rdx
    
no_swap:
    mov r14, rcx
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
