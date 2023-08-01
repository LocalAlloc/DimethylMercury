bits 16 ; 16-bit code
org 0x7C00 ; Set image base

xor ax, ax
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax ; Clear segment registers

mov esp, 0x7C00
mov ebp, esp ; Stack starts at bootloader base

jmp 0x0000:load ; Clear CS, some BIOSes set CS=0x7C0 and IP=0

load:
    xor ah, ah   ; Set video mode function (0x00)
    mov al, 0x13 ; 320x200, 256 colors
    int 0x10

    rdtsc
    mov word [seed], ax ; Set seed to timestamp counter

    mov ax, 0xA000    ; Video memory address
    mov es, ax        ; Set segment
    mov di, 320*200-1 ; Set index
    mov cl, 32        ; Set color index 32 in VGA palette

    ; Set text position
    mov si, text
    call show_text

    jmp draw_loop       ; Enter draw loop

; AX = Return value, BX = Seed
xorshift16:
    mov ax, bx ; Load seed into AX
    shl ax, 7 ; Shift
    xor bx, ax ; XOR...
    mov ax, bx
    shr ax, 9
    xor bx, ax
    mov ax, bx
    shl ax, 8
    xor bx, ax
    mov ax, bx
    ret

draw_loop:
    mov bx, word [seed]
    call xorshift16
    xor word [seed], ax

    xor dx, dx
    mov bx, 3
    div bx
    mov ax, dx
    add al, cl ; Add counter to make a rainbow effect

    mov [es:di], al ; Set pixel
    dec di          ; Decrement index
    cmp di, 0xFFFF  ; Check if index went out of bounds
    jne draw_loop   ; Continue if not ended

    mov di, 320*200-1 ; Reset index
    cmp cl, 51        ; Check if above color index 51 in VGA palette
    jg .rainbow_end
    inc cl            ; Increment color index
    jmp draw_loop
.rainbow_end:
    inc ch        ; Increment counter
    mov cl, 32    ; Set color index 32 in VGA palette
    jmp draw_loop

seed:
    dw 4096

text:
    db "Hello, Bootloader!", 0

show_text:
    ; Save AX and BX for later
    push ax
    push bx

    ; Setup Data Segment
    mov ax, 0x07C0
    mov ds, ax

    ; Teletype Output
    mov ah, 0x0E

    printLoop:
        ; Load the character from memory
        lodsb

        ; If it is the null terminator, exit the loop
        cmp al, 0
        je returnFirstColorText

        ; Print the character
        int 0x10
        jmp printLoop

    returnFirstColorText:
        ; Restore AX and BX values
        pop bx
        pop ax

        ret

times 510 - ($ - $$) db 0
dw 0xAA55 ; Signature