global ft_list_remove_if

struc Node
    .data resq 1
    .next resq 1
endstruc

section .text
ft_list_remove_if:
    test    rdi, rdi
    jz      done
    test    rsi, rsi
    jz      done
    test    rdx, rdx
    jz      done
    test    rcx, rcx
    jz      done

    mov     r8, [rdi + Node.next]
    test    r8, r8
    jz      done

list_loop:
    test    rdi, rdi
    jz      done

    mov     r9, [rdi + Node.data]
    mov     r10, rsi

    push    rdi
    push    rsi
    push    r8
    sub     rsp, 8

    mov     rdi, r9 
    mov     rsi, r10
    call    rdx

    add     rsp, 8
    pop     r8
    pop     rsi
    pop     rdi

    cmp     rax, 0
    je      remove_data
    mov     rdi, r8
    mov     r8, [r8 + Node.next]
    jmp     list_loop

remove_data:
    push    rsi
    push    rdx
    push    r8
    sub     rsp, 8

    mov     rdi, [rdi  + Node.data]
    call    rdx

    add     rsp, 8
    pop     r8
    pop     rdx
    pop     rsi

    mov     rdi, r8
    mov     r8, [r8 + Node.next]
    jmp     list_loop

done:
    ret
