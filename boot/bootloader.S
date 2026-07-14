.code16
.global _start

_start:
    cli
    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movw $0x7C00, %sp
    sti

    movw $ascii_calc, %si

print_loop:
    lodsb
    cmpb $0, %al
    je load_kernel
    movb $0x0E, %ah
    int $0x10
    call delay
    jmp print_loop

delay:
    movw $0xFFFF, %cx
d1:
    loop d1
    ret

load_kernel:
    xorw %ax, %ax
    movw %ax, %es
    movw $0x1000, %bx
    
    movb $0x02, %ah
    movb $20, %al
    movb $0, %ch
    movb $2, %cl
    movb $0, %dh
    movb $0x80, %dl
    int $0x13
    
    jc disk_error
    ljmp $0x0000, $0x1000

disk_error:
    movw $error_msg, %si
error_loop:
    lodsb
    cmpb $0, %al
    je halt
    movb $0x0E, %ah
    int $0x10
    jmp error_loop

halt:
    cli
    hlt

ascii_calc:
    .ascii "CalculatorOS\r\n\0"

error_msg:
    .ascii "Disk Read Fail!\0"

.org 510
.word 0xAA55
