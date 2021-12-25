BITS 16

mov ax, 07c0h
mov ds, ax
mov ax, 0b800h
mov es, ax
or bx, bx
mov ax, 3
int 10h

mov ax, 0000h
mov cx, 990
l:
	mov [es:bx], ax
	add bx, 2
	loop l

push bx
push bx
mov di, key
mov cx, len_key
mov ah, 0f0h
k:
	mov al, [di]
	mov [es:bx], ax
	add bx, 2
	inc di
	loop k
xor dx, dx
xor ax, ax

int 16h
mov dl, al

mov cx, len_key
pop bx
mov ax, 00000h
c:
	mov [es:bx], ax
	add bx, 2
	loop c

pop bx
sub bx, 10
mov si, name
mov cx, len_name
n:
	mov al, [si]
	xor al, dl
	mov ah, [si]
	and ah, 31
	mov [es:bx], ax
	add bx, 2
	inc si
	loop n
jmp $
key: db 'Enter your key: '
len_key: equ $-key
name: db 'Komogorov  Kirill KKCO-03-20'
len_name: equ $-name
times 510 - ($-$$) db 0
dw 0xaa55
times 1024 * 1024 - ($-$$) db 0
