[SECTION .text]
[BITS 32]
extern kernel_main

global _start
_start:
; set 8259a chip and enable interruption
.config_8a59a:
    ; send ICW1 to master chip
    mov al, 11h
    out 20h, al

    ; send ICW1 to slave chip
    out 0a0h, al

    ; send ICW2 to master chip
    mov al, 20h
    out 21h, al

    ; send ICW2 to slave chip
    mov al, 28h
    out 0a1h, al

    ; send ICW3 to master chip
    mov al, 04h
    out 21h, al

    ; send ICW3 to master chip
    mov al, 02h
    out 0A1h , al

    ; send ICW4 to master chip
    mov al, 003h
    out 021h, al

    ; send ICW4 to slave chip
    out 0A1h, al
 
    ;Mask all interrupts, only receive keyboard interrupts
.enable_8259a_main:
    mov al, 11111000b
    out 21h, al

    ; disable interruptions from slave chip
.disable_8259a_slave:
    mov al, 00111111b
    out 0A1h, al

    ; call C program
.enter_c_word:
    call kernel_main

    jmp $