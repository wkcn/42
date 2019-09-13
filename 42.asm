ITEM_SIZE equ 128
ORD_0 equ 48
ORD_MINUS equ 45

_start:
    mov ax, 07c0h 
    mov ds, ax
    mov ss, ax
    mov sp, 100h

    ;清屏
    mov ax, 3
    int 10h

    mov ax, 0B800h
    mov es, ax

    mov cx, [msgLen]
    mov si, message
    mov di, 0*2*80 + 0

    call print_str

    ; a
    mov bl, 0
    call get_item 

    mov si, a_str
    mov cx, [a_len]
    call read_num

    mov di, 1*2*80 + 0
    call print_num

    ; b
    mov bl, 1
    call get_item 

    mov si, b_str
    mov cx, [c_len]
    call read_num

    mov di, 2*2*80 + 0
    call print_num

    ; c
    mov bl, 2
    call get_item 

    mov si, c_str
    mov cx, [c_len]
    call read_num

    mov di, 3*2*80 + 0
    call print_num

    jmp $

%macro PRINT_CHAR 0
    mov ah, 07H
    mov [es:di], ax
%endmacro

print_str:
    ; al, si, di
    print_str_loop:
        mov al, byte [si]
        PRINT_CHAR
        inc si
        add di, 2
    loop print_str_loop 
    ret

get_item:
    ; input: bx
    ; return address in ax
    mov al, ITEM_SIZE 
    mul bl 
    add ax, BUFFER
    ret

read_num:
    ; inputs: ax: address of num 
    ;         cx: len
    ;         si: the address of str
    ; byte minus, byte len, byte...
    ; use ax, bx, cx, di, si
    push bx
    push di
    mov di, ax

    ; sign
    mov byte [di], 0
    mov bl, byte [si]
    cmp bl, ORD_MINUS
    ; choose a[-1]
    add si, cx
    dec si
    jne NO_MINUS
    ; it is a minus
    mov byte [di], 1
    dec cx

    NO_MINUS:
    inc di
    ; len
    mov byte [di], cl
    inc di
    ; it seems that `sub` will break the loop
    read_str_loop:
        cmp cx, 0
        je read_str_end
        mov bl, byte [si]
        sub bl, ORD_0
        mov byte [di], bl
        dec si
        inc di
        dec cx
        jmp read_str_loop
    read_str_end:
    pop di
    pop bx
    ret

print_num:
    ; inputs: ax: address
    ;         di: print position
    mov si, ax
    cmp byte [si], 1 
    jne print_num_no_minus
    mov al, ORD_MINUS
    PRINT_CHAR
    add di, 2
    print_num_no_minus:
    inc si
    mov cl, byte [si]
    mov ch, 0
    add si, cx
    print_num_i_loop:
        cmp cx, 0
        je print_num_i_out 
        mov al, byte [si]
        add al, ORD_0
        PRINT_CHAR
        add di, 2
        dec si
        dec cx
        jmp print_num_i_loop
    print_num_i_out:
    ret

DATA:
    message db 'The answer to life, the universe, and everything:'
    msgLen dw $-message
    a_str db "-80538738812075974"
    a_len dw $-a_str
    b_str db "80435758145817515"
    b_len dw $-b_str
    c_str db "12602123297335631"
    c_len dw $-c_str
BUFFER:
times 510-($-$$) db 0  
dw  0xaa55
