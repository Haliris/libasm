global ft_list_remove_if
extern free
struc Node
    .data resq 1
    .next resq 1
endstruc
section .text
ft_list_remove_if:
    test    rsi, rsi        ; Check if data_ref is NULL
    jz      done
    test    rdx, rdx        ; Check if cmp function is NULL
    jz      done
    test    rcx, rcx        ; Check if free_fct is NULL
    jz      done
    
    ; Save callee-saved registers
    push    r12
    push    r13
    push    r14
    push    r15
    
    mov     r12, rcx        ; Store free_fct in r12
    mov     r13, rdx        ; Store cmp function in r13
    mov     r14, rsi        ; Store data_ref in r14
    mov     r15, rdi        ; Store list head pointer in r15
    
    xor     r10, r10        ; Initialize prev pointer to NULL
    mov     rdi, [r15]      ; Load the head node pointer
    
    test    rdi, rdi        ; Check if the list is empty
    jz      cleanup
    
    mov     r8, [rdi + Node.next]  ; Get next node
    
list_loop:
    test    rdi, rdi        ; Check if current node is NULL
    jz      cleanup
    
    mov     r9, [rdi + Node.data]  ; Get data pointer from current node
    
    push    rdi             ; Save current node
    push    r8              ; Save next node
    push    r10             ; Save prev node
    
    mov     rdi, r9         ; Pass data pointer as first arg
    mov     rsi, r14        ; Pass data_ref as second arg 
    call    r13             ; Call cmp function
    
    pop     r10             ; Restore prev node
    pop     r8              ; Restore next node
    pop     rdi             ; Restore current node
    
    test    rax, rax        ; Check comparison result
    je      continue_loop   ; If not equal (0), continue
    
    ; Node needs to be removed
    push    rdi             ; Save node to be freed
    push    r8              ; Save next node
    push    r10             ; Save prev node
    push    r14             ; Save data_ref
    
    mov     rdi, [rdi + Node.data] ; Get data pointer to free
    call    r12             ; Call free_fct on data
    
    pop     r14             ; Restore data_ref
    pop     r10             ; Restore prev node
    pop     r8              ; Restore next node
    pop     rdi             ; Restore node to be freed
    
    ; Relink nodes
    test    r10, r10        ; Check if we're removing the head
    jz      update_head
    
    mov     [r10 + Node.next], r8  ; Link prev->next to next
    jmp     free_node
    
update_head:
    mov     [r15], r8       ; Update head pointer
    
free_node:
    push    r8              ; Save next node
    push    r10             ; Save prev node
    call    free wrt ..plt  ; Free the current node
    pop     r10             ; Restore prev node
    pop     r8              ; Restore next node
    
    mov     rdi, r8         ; Move to next node
    test    r8, r8          ; Check if next is NULL
    jz      cleanup
    mov     r8, [r8 + Node.next]  ; Update next->next
    jmp     list_loop
    
continue_loop:
    mov     r10, rdi        ; Update prev to current
    mov     rdi, r8         ; Move to next node
    test    r8, r8          ; Check if next is NULL
    jz      cleanup
    mov     r8, [r8 + Node.next]  ; Update next->next
    jmp     list_loop
    
cleanup:
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    
done:
    ret
