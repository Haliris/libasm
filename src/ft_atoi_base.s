global ft_atoi_base
extern ft_strlen

//Check base (rsi)
ft_atoi_base:
    xor     rax, rax
    xor     r8, r8
    xor     r10, r10
    mov     r11, rdi //string to convert
    mov     rdi, rsi
    jmp     check_base

check_base:
    call    ft_strlen wrt ..plt// move rsi to top of the stack
    cmp     rax, 0
    jz      done // is this correct instruction
    cmp     rax, 1
    jz      done // is this correct instruction
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
    je      done

convert:
    cmp byte [r9 + r10], 0
    je  done

done:
    ret
