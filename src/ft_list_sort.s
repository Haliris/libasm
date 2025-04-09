global ft_list_sort
extern ft_list_size
extern printf
struc Node
    .data resq 1
    .next resq 1
endstruc
section .rodata. 
    fmt db "Comparison returned: %d", 10, 0
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
    mov     rax, [r14 + Node.next]
    test    rax, rax
    jz      next_node
    mov     rdi, [r14 + Node.data]
    mov     rsi, [rax + Node.data]
    push    r14
    push    rbx
    push    r15
    push    rax
    sub     rsp, 8
    call    r12
    add     rsp, 8
    mov     rdi, rax        ; Store return value in rdi (for printf)
    push    rax             ; Preserve rax (printf may clobber it)
    mov     rsi, rax        ; Also pass it as argument (optional)
    lea     rdi, [rel fmt]  ; Load format string address
    xor     eax, eax        ; Clear AL (no floating-point args)
    call    printf wrt ..plt         ; Print the return value
    pop     rax             ; Restore rax
    pop     rax
    pop     r15
    pop     rbx
    pop     r14
    cmp     rax, 0
    jg      do_swap
    jmp     no_swap
do_swap:
    mov     rsi, [rax + Node.data]
    mov     rdx, [r14 + Node.data]
    mov     [r14 + Node.data], rsi
    mov     [rax + Node.data], rdx
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
