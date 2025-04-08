section .text
global ft_atoi_base
extern ft_strlen


ft_atoi_base:
    xor     rax, rax
    xor     r8, r8
    xor     r9, r9
    xor     r10, r10
    xor     r11, r11
    xor     r12, r12
    xor     r13, r13
    xor     r14, r14
    mov     r11, rdi
    mov     rdi, rsi

    lea     rcx, [rel char_table]
    mov byte [rcx + 9], 1
    mov byte [rcx + 10], 1
    mov byte [rcx + 11], 1
    mov byte [rcx + 12], 1
    mov byte [rcx + 13], 1
    mov byte [rcx + 32], 1
    mov byte [rcx + 43], 1
    mov byte [rcx + 45], 1

    cmp     byte [rdi], 45
    jmp     check_base


check_base:
    call    ft_strlen wrt ..plt
    cmp     rax, 0
    jz      done_error
    cmp     rax, 1
    jz      done_error
    mov     r13, rax
    xor     rax, rax
    jmp     base_outer_loop

base_outer_loop:
    mov     al, [rdi + r8]
    test    al, al
    je      skip_charset
    movzx   rdx, al
    cmp     byte [rcx + rdx], 1
    je      done_error
    mov     r9, r8
    inc     r9

base_inner_loop:
    mov     bl, [rdi + r9]
    test    bl, bl
    je      next_i
    cmp     al, bl
    je      done_error
    inc     r9
    jmp     base_inner_loop

next_i:
    inc r8
    jmp base_outer_loop

skip_charset:
    mov     rax, 0
    xor     r10, r10
    mov     al, [r11]
    test    al, al
    je      done
    movzx   rdx, al
    cmp     byte [rcx + rdx], 1
    jne     check_neg_sign
    inc     r11
    cmp     al, 45
    jne     skip_charset
    inc     r12
    jmp     skip_charset
    
check_neg_sign:
    and     r12, 1
    jne     set_neg_sign
    mov     r12, 1
    jmp     convert

set_neg_sign:
    mov r12, -1
    jmp convert

convert:
    cmp     byte [r11 + r10], 0
    je      done
    jmp     check_in_base

check_in_base:
    xor     r9, r9
    xor     rax, rax
    mov     al, [r11 + r10]
    jmp     check_in_base_loop

check_in_base_loop:
    cmp     byte [rdi + r9], 0
    je      done
    mov     bl, [rdi + r9]
    cmp     al, bl
    je      add_value
    inc     r9
    jmp     check_in_base_loop

add_value:
    imul    r14, r13
    add     r14, r9
    inc     r10
    jmp     convert

done:
    imul    r14, r12
    mov     rax, r14
    ret

done_error:
    xor rax, rax
    ret

section .data
char_table:
    times 256 db 0

