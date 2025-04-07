global ft_atoi_base
extern ft_strlen

section .data
char_table:
    times 256 db 0

    //whitespaces
    mov byte [char_table + 9], 1
    mov byte [char_table + 10], 1
    mov byte [char_table + 11], 1
    mov byte [char_table + 12], 1
    mov byte [char_table + 13], 1
    mov byte [char_table + 32], 1

    //signs
    mov byte [char_table + 43], 1
    mov byte [char_table + 45], 1

//Check base (rsi)
ft_atoi_base:
    xor     rax, rax
    xor     r8, r8
    xor     r9, r9
    xor     r10, r10
    xor     r11, r11
    xor     r12, r12
    xor     r13, r13
    mov     r11, rdi //string to convert
    mov     rdi, rsi
    mov     r12, 1
    cmp     byte [rdi], 45
    je      set_neg_sign
    jmp     check_base

set_neg_sign:
    mov r12, -1

check_base:
    call    ft_strlen wrt ..plt// move rsi to top of the stack
    cmp     rax, 0
    jz      done_error // is this correct instruction
    cmp     rax, 1
    jz      done_error // is this correct instruction
    mov     r13, rax
    xor     rax, rax
    jmp     base_outer_loop

base_outer_loop:
    mov     al, [rdi + r8]
    test    al, al
    je      convert
    mov     r9, r8
    inc     r9

base_inner_loop:
    mov     bl, [rdi + r9]
    test    bl, bl
    je      next_i
    cmp     al, bl
    je      done_error
    movzx   r12, bl
    mov     al, [char_table + rax]
    test    al, al
    jne     done_error
    inc     r9
    jmp     base_inner_loop

next_i:
    inc r8
    jmp base_outer_loop

convert:
    cmp     byte [r9 + r10], 0
    je      done
    jmp     check_value

check_value:
    mov     al, [r9 + 10]
    cmp     al, '0'
    jb      check_lowercase
    cmp     al, '9'
    ja      check_uppercase
    movzx   eax, al
    sub     eax, '0'
    jmp     add_value


check_lowercase:
    cmp     al, 'a'
    jb      check_uppercase
    cmp     al, 'z'
    ja      check_uppercase
    movzx   eax, al
    sub     eax, 'a'
    add     eax, 10
    jmp     add_value

check_uppercase:
    movzx   eax, al
    sub     eax, 'A'
    add     eax, 10
    jmp     add_value

add_value:
    imul    rax, r13
    add     rax, eax
    inc     r10
    jmp     convert

done:
    imul rax, r12
    ret

done_error:
    xor rax, rax
    ret
