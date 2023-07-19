[ORG  0x7c00]

[SECTION .data]
BOOT_MAIN_ADDR equ 0x500

[SECTION .text]
[BITS 16]
global boot_start
boot_start:
    ; Set the screen mode to text mode, clear the screen
    mov ax, 3
    int 0x10

    ; Read set up into memory
    mov edi, BOOT_MAIN_ADDR ; the destination
    mov ecx, 1      ; Start from which sector
    mov bl, 2       ; Sector nums
    call read_hd

    ; jump to main address
    mov     si, jmp_to_setup
    call    print

    jmp     BOOT_MAIN_ADDR

read_hd:
    ; 0x1f2 8bit
    ; Specifies the number of sectors to read or write
    mov dx, 0x1f2
    mov al, bl
    out dx, al

    ; 0x1f3 8bit
    ; The eighth digit of the iba address 0-7
    inc dx
    mov al, cl
    out dx, al

    ; 0x1f4
    ; The eighth digit of the iba address 0-7
    inc dx
    mov al, ch
    out dx, al

    ; 0x1f5 8bit
    ; The eighth digit of the iba address 16-23
    inc dx
    shr ecx, 16
    mov al, cl
    out dx, al

    ; 0x1f6 8bit
    ; 0-3 24-27 of iba address
    ; 4 0 as master disk, 1 as slave disk
    ; 5、7 is fixed to 1
    ; 6 0 represent CHS mode，1 represent LAB mode
    inc dx
    shr ecx, 8
    and cl, 0b1111
    mov al, 0b1110_0000     ; LBA mode
    or al, cl
    out dx, al

    ; 0x1f7 8bit  command or status port
    inc dx
    mov al, 0x20
    out dx, al

    ; set loop times，represent sector numbers
    mov cl, bl
.start_read:
    push cx     ; save cx

    call .wait_hd_prepare
    call read_hd_data

    pop cx      ; recover cx

    loop .start_read

.return:
    ret

; Wait until the status of the hard disk is: not busy, data is ready
.wait_hd_prepare:
    mov dx, 0x1f7

.check:
    in al, dx
    and al, 0b1000_1000
    cmp al, 0b0000_1000
    jnz .check

    ret

; Read the hard disk, read two bytes at a time, read 256 times, just read one sector
read_hd_data:
    mov dx, 0x1f0
    mov cx, 256

.read_word:
    in ax, dx
    mov [edi], ax
    add edi, 2
    loop .read_word

    ret

; print function
print:
    mov ah, 0x0e
    mov bh, 0
    mov bl, 0x01
.loop:
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10

    inc si
    jmp .loop
.done:
    ret

jmp_to_setup:
    db "jump to setup...", 10, 13, 0

times 510 - ($ - $$) db 0
db 0x55, 0xaa