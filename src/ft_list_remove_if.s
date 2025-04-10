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

    push    r12
    push    r13
    push    r14
    push    r15
    
    mov     r12, rcx        ; free_fct
    mov     r13, rdx        ; cmp function
    mov     r14, rsi        ; data_ref
    mov     r15, rdi        ; list head pointer
    
    xor     r10, r10        ; prev pointer
    mov     rdi, [r15]      ; head node pointer
    
    test    rdi, rdi
    jz      cleanup
    
    mov     r8, [rdi + Node.next]  ; next node
    
list_loop:
    test    rdi, rdi
    jz      cleanup
    
    mov     r9, [rdi + Node.data]
    
    push    rdi
    push    r8
    push    r10
    sub     rsp, 8
    
    mov     rdi, r9
    mov     rsi, r14
    call    r13
    
    add     rsp, 8
    pop     r10
    pop     r8
    pop     rdi
    
    cmp     rax, 0
    je      continue_loop
    
    push    rdi
    push    r8
    push    r10
    push    r14
    
    mov     rdi, [rdi + Node.data]
    call    r12
    
    pop     r14
    pop     r10
    pop     r8
    pop     rdi
    
    test    r10, r10
    jz      update_head
    
    mov     [r10 + Node.next], r8
    jmp     free_node
    
update_head:
    mov     [r15], r8
    
free_node:
    push    r8
    push    r10
    call    free wrt ..plt
    pop     r10
    pop     r8
    
    mov     rdi, r8
    test    r8, r8
    jz      cleanup
    mov     r8, [r8 + Node.next]
    jmp     list_loop
    
continue_loop:
    mov     r10, rdi
    mov     rdi, r8
    test    r8, r8
    jz      cleanup
    mov     r8, [r8 + Node.next]
    jmp     list_loop
    
cleanup:
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    
done:
    ret
