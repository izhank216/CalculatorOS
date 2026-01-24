BITS 16
ORG 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov si, ascii_calc
print_loop:
    lodsb
    cmp al, 0
    je load_kernel
    mov ah, 0x0E
    int 0x10
    call delay
    jmp print_loop

delay:
    mov cx, 0xFFFF
d1:
    loop d1
    ret

load_kernel:
    mov bx, 0x1000
    mov ah, 0x02
    mov al, 20
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x80
    int 0x13
    jmp 0x1000:0x0000

ascii_calc db "CalculatorOS",0

times 510-($-$$) db 0
dw 0xAA55
