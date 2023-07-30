[BITS 16]
[ORG 0x7C00]

WSCREEN equ 320
HSCREEN equ 200

call setup
call palette

setup:
    ; Setup Mode 13h
    mov ah, 0x00
    mov al, 0x13
    int 0x10

    ; Setup Video Segment
    push 0xA000
    pop es

    ; Write graphics pixel function
    mov ah, 0x0C

    ; Setup some registers
    xor al, al   ; AL = Color
    xor bx, bx   ; BX = Page
    xor cx, cx   ; CX = X
    mov dx, 0x08 ; DX = Y

    ; Initialize FPU
    fninit

    call text

    ret

;-------------------------------------

crash:
	;Restart the program again from the beginning
	mov ax, 0x03
	int 0x19
		
;-------------------------------------	
reset:
    ; Start all over again
    xor cx, cx
    mov dx, 0x08

    ; COLOR++
    inc word [color]

palette:
    ; Store CX and DX to use it later
    ; using FPU instructions
    mov word [x], cx
    mov word [y], dx

    ; Payload's execution system
    cmp word [color], 2048
    ja crash

    cmp word [color], 1792
    ja mandelbrot

    jmp setpixel

;-------------------------------------

setpixel:
    ; If the pixel reaches the border at the right
    ; Go to the next line
    cmp cx, WSCREEN
    jae nextline

    ; If the screen gets completely full of pixels
    ; Reset again
    cmp dx, HSCREEN
    jae reset

    ; Set Pixel
    int 0x10

    ; Go to the next pixel
    inc cx
    jmp palette

    ret

;-------------------------------------

nextline:
    xor cx, cx
    inc dx

    jmp palette

;-------------------------------------

text:
    ; Save AX and BX for later
    push ax
    push bx

    ; Setup Data Segment
    mov ax, cx
    mov ds, ax

    ; Teletype Output
    mov ah, 0x0E

    ; Position for text display
    mov di, 320*15  ; Move text further down to create space

    ; Copy text to video memory
    mov si, string
    copyLoop:
        lodsb   ; Load character from memory
        stosb   ; Store character in video memory
        test al, al ; Check if null terminator
        jnz copyLoop ; Continue copying if not

    ; Restore AX and BX values
    pop bx
    pop ax

    ret

;-------------------------------------

mandelbrot:
    call calculate_mandelbrot
    call set_pixel

    ; Wait for a key press
    call wait_for_key

    ; Handle key press
    mov ah, 0 ; Clear keyboard buffer
    int 0x16

    cmp al, 0x48 ; Up arrow key
    je move_up
    cmp al, 0x50 ; Down arrow key
    je move_down
    cmp al, 0x4B ; Left arrow key
    je move_left
    cmp al, 0x4D ; Right arrow key
    je move_right
    cmp al, 0x49 ; Page Up key
    je zoom_in
    cmp al, 0x51 ; Page Down key
    je zoom_out

    jmp mandelbrot

move_up:
    sub dword [posY], 10
    jmp mandelbrot

move_down:
    add dword [posY], 10
    jmp mandelbrot

move_left:
    sub dword [posX], 10
    jmp mandelbrot

move_right:
    add dword [posX], 10
    jmp mandelbrot

zoom_in:
    shl dword [zoom], 1
    jmp mandelbrot

zoom_out:
    shr dword [zoom], 1
    jmp mandelbrot

;-------------------------------------

; Mandelbrot calculation
calculate_mandelbrot:
    ; Convert pixel coordinates to Mandelbrot coordinates
    fld dword [zoom]
    fild dword [posX]
    fdiv
    fst dword [zx]

    fld dword [zoom]
    fild dword [posY]
    fdiv
    fst dword [zy]

    ; Initialize iteration count
    mov dword [iteration], 0

mandelbrot_loop:
    ; Calculate the next iteration
    fld dword [zx]
    fld dword [zx]
    fmul
    fld dword [zy]
    fld dword [zy]
    fmul
    fsub
    fstp dword [zx]

    fld dword [zy]
    fld dword [zx]
    fmul
    fld dword [zx]
    fld dword [zy]
    fmul
    fadd
    fstp dword [zy]

    ; Check if the magnitude exceeds the escape radius
    fld dword [zx]
    fmul
    fld dword [zy]
    fmul
    fadd
    fsqrt
    fcomp dword [escape_radius_squared]
    fstsw ax
    sahf

    ; If the magnitude exceeds the escape radius or the maximum iteration is reached, exit the loop
    jae mandelbrot_done
    cmp dword [iteration], 255
    jge mandelbrot_done

    ; Increase the iteration count and repeat the loop
    inc dword [iteration]
    jmp mandelbrot_loop

mandelbrot_done:
    ret

; Set pixel color based on iteration count
set_pixel:
    mov ah, 0x0C
    mov al, byte [iteration]
    int 0x10
    ret

; Wait for a key press
wait_for_key:
    xor ah, ah
    int 0x16
    ret

;-------------------------------------

; Define some variables for Mandelbrot fractal
posX: dd 100
posY: dd 100
zoom: dd 100
zx: dd 0.0
zy: dd 0.0
iteration: dd 0
escape_radius_squared: dd 4.0

;-------------------------------------

; Define some variables for text display
string: db "IT LOOKS LIKE YOU'RE HAVING A BAD DREAM!", 0x00
color: dw 0
x: dw 100
y: dw 100

;-------------------------------------

; MBR Signature
times 510 - ($ - $$) db 0
dw 0xAA55
