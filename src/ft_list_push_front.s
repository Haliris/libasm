global ft_list_push_front
extern malloc
extern __errno_location

struc Node
    .data resq 1
    .next resq 1
endstruc

section .text
ft_list_push_front:
    push    rbx
    push    r12

    mov     r12, rdi
    mov     rbx, rsi

    mov     rdi, 16
    call    malloc wrt ..plt
    test    rax, rax
    jz      error

    mov     [rax + Node.data], rbx
    mov     rcx, [r12]
    mov     [rax + Node.next], rcx
    mov     [r12], rax
    jmp     done

error:
    neg     rax
    mov     rdi, rax
    call    __errno_location wrt ..plt
    mov     [rax], rdi
    mov     rax, -1
    pop     r12
    pop     rbx
    ret

done:
    pop r12
    pop rbx
    xor rax, rax
    ret

