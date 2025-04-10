global ft_list_remove_if
extern free

struc Node
    .data resq 1
    .next resq 1
endstruc

section .text
ft_list_remove_if:
    test    rsi, rsi
    jz      done
    test    rdx, rdx
    jz      done
    test    rcx, rcx
    jz      done
    xor     r8, r8
    xor     r9, r9
    xor     r10, r10
    xor     r11, r11
    xor     r12, r12
    xor     r13, r13
    mov     r8, [rdi + Node.next]
    mov     r11, rdx
    mov     r12, rcx

list_loop:
    test    rdi, rdi
    jz      done

    mov     r9, [rdi + Node.data]

    push    rdi
    push    r8
    push    r10
    push    r12

    mov     rdi, r9
    call    r11

    pop     r12
    pop     r10
    pop     r8
    pop     rdi

    cmp     rax, 0
    je      remove_data
    xor     r9, r9
    mov     r10, rdi
    mov     rdi, r8
    test    r8, r8
    jz      list_loop
    mov     r8, [r8 + Node.next]
    jmp     list_loop

remove_data:
    mov     r13, rdi
    push    rsi
    push    rdx
    push    r8
    push    r10
    push    r13
    sub     rsp, 8

    mov     rdi, [rdi  + Node.data]
    call    r12

    add     rsp, 8
    pop     r13
    pop     r10
    pop     r8
    pop     rdx
    pop     rsi
    mov     rdi, r13

relink_nodes:
    push    rdx
    push    r8
    push    rsi
    push    r10
    push    r11
    push    r12

    call    free wrt ..plt

    pop     r12
    pop     r11
    pop     r10
    pop     rsi
    pop     r8
    pop     rdx

    test    r10, r10
    jz      update_head
    mov     [r10 + Node.next], r8
    jmp     next_node

update_head:
    mov [rdi + Node.next], r8
    jmp next_node

next_node:
    mov     r10, rdi
    mov     rdi, r8
    test    r8, r8
    jz      list_loop
    mov     r8, [r8 + Node.next]
    jmp     list_loop

done:
    ret
