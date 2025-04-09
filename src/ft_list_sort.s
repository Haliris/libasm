global ft_list_sort
extern ft_list_size

struc Node
    .data resq 1
    .next resq 1
endstruc

section .text
ft_list_sort:
    push    r12,
    xor     rax, rax
    xor     r8, r8
    xor     r9, r9

    test    rdi, rdi
    jz      done
    test    rsi, rsi
    jz      done

    mov     rbx, rdi
    mov     rcx, [rbx + 8]
    test    rcx, rcx
    jz      done
    mov     r12, rsi

    mov     rdi, [rbx]
    call    ft_list_size
    test    rax, rax
    jz      done
    mov     r8, rax
    and     r8, 1
    jne     uneven_size_inc
    shr     rax, 1
    mov     r8, rax
    xor     rax, rax
    jmp     compare_outer_loop

uneven_size_inc:
    inc     rax
    shr     rax, 1
    mov     r8, rax
    xor     rax, rax
    jmp     compare_outer_loop

compare_outer_loop:
    cmp     r9, r8
    je      done
    jmp     compare_inner_loop

compare_inner_loop:
    test    rcx, rcx
    jz      next_node
    push    rbx
    push    rcx

    mov     rdi, [rbx + Node.data]
    mov     rsi, [rcx + Node.data]
    call    r12

    pop     rcx
    pop     rbx
    cmp     rax, 0
    jg      swap_data
    mov     rcx, [rcx + Node.next]
    jmp     compare_inner_loop

swap_data:
    push    rdx
    mov     rdx, [rbx + Node.data]
    mov     rdi, [rcx + Node.data]
    mov     [rbx + Node.data], rdi
    mov     [rcx + Node.data], rdx
    pop     rdx
    mov     rcx, [rcx + Node.next]
    jmp     compare_inner_loop

next_node:
    mov     rbx, [rbx + Node.next]
    mov     rcx, [rbx + Node.next]
    inc     r9
    jmp     compare_outer_loop

done:
    pop r12,
    ret

