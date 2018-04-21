; Function to calculate the string length
; Assumes that string starting address is in eax
; returns the length in eax
fun_stringLen:
	push ebx
	mov ebx,eax

nextChar:
	cmp byte[eax], 0
	jz finished
	inc eax
	jmp nextChar

finished:
	sub eax,ebx
	pop ebx
	ret



; Printing String
fun_sprint:
	push edx
	push ecx
	push ebx
	push eax
	call fun_stringLen

	mov edx,eax ; edx now has the length of the string
	pop eax ; eax has the address of the string

	mov ecx, eax
	mov ebx, 1
	mov eax, 4
	int 80h

	pop ebx
	pop ecx
	pop edx
	ret


fun_printLineFeed:
	call fun_sprint

	push eax
	mov eax, 0Ah ; \n
	push eax
	mov eax, esp
	call fun_sprint
	pop eax ; remove \n
	pop eax ; restore eax
	ret

; Pass the number to printed in eax 
fun_iprint:
	push eax
	push ecx
	push edx
	push esi
	mov ecx, 0

divideLoop:
	inc ecx ; stores the number of characters to print
	mov edx, 0
	mov esi, 10
	idiv esi
	add edx,48 ; edx stores the remainder, add the ascii to make it integer
	push edx
	cmp eax, 0
	jnz divideLoop

printLoop:
	dec ecx
	mov eax, esp
	call fun_sprint
	pop eax
	cmp ecx, 0
	jnz printLoop

	pop esi ; pop all preserved and return
	pop edx
	pop ecx
	pop eax
	ret

; To print Integer with a '\n'
fun_iPrintLF:
	call fun_iprint

	push eax
	mov eax, 0Ah
	push eax
	mov eax, esp
	call fun_sprint
	pop eax
	pop eax
	ret

; pass the address of input variable to eax
fun_readString:
	mov edx, 20 ; strings are of 20 bytes
	mov ecx, eax
	mov ebx, 0
	mov eax, 3
	int 80h
	ret

fun_readInteger:
	push ebx
	push ecx
	push edx
	push esi
	mov esi, eax
	mov eax, 0
	mov ecx, 0

multiply:
	xor ebx, ebx ; turn ebx to zero
	mov bl, [esi + ecx] ; moivng 1 byte to bl
	cmp bl, 48
	jl fin
	cmp bl, 57
	jg fin
	cmp bl, 10 ; '\n'
	je fin
	cmp bl, 0  ; '\0'
	jz fin
	sub bl, 48
	add eax, ebx
	mov ebx, 10
	mul ebx
	inc ecx
	jmp multiply

fin:
	mov ebx, 10
	div ebx
	pop esi
	pop edx
	pop ecx
	pop ebx
	ret

; arg1 addr is in eax, arg2 in ebx and arg3 in ecx
; first two bytes represent the size in rows X columns
fun_matrixAdd:
	push ecx
	push esi
	push edi
	mov esi,eax
	mov edi,ecx 
	lodsw
	stosw
	lodsw
	stosw
addLoop:	
	lodsw
	cmp ax,-1
	jz fin2
	add ax, [ebx]
	stosw 
	add ebx, 2
	jmp addLoop

fin2:
	mov edi,-1
	pop edi
	pop esi
	pop ecx
	ret

fun_matCopy:
	push esi
	push edi
	mov esi,eax
	mov edi,ebx
	lodsw
	stosw
	lodsw
	stosw
cpyl:
	lodsw
	cmp ax,-1
	jz matFin
	stosw
	jmp cpyl

matFin:
	pop edi
	pop esi
	ret

fun_printEnter:
	push eax
	mov eax, 0Ah
	push eax
	mov eax, esp
	call fun_sprint
	pop eax
	pop eax
	ret

fun_printSpace:
	push eax
	mov eax, 20h
	push eax
	mov eax, esp
	call fun_sprint
	pop eax
	pop eax
	ret

; eax has the address
; ecx has rows
; edx has columns
fun_matPrint:
	push esi
	push ebx
	mov esi,eax

rows:
	mov bx, dx ; the number of columns
columns:
	mov eax, 0
	lodsw 
	call fun_iprint
	call fun_printSpace
	dec bx
	cmp bx,0
	jnz columns
	call fun_printEnter
	dec cx
	cmp cx,0
	jnz rows

fin_matPrint:
	pop ebx
	pop esi
	ret


fun_exit:
	mov ebx, 0
	mov eax, 1
	int 80h
	ret
