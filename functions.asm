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

fun_exit:
	mov ebx, 0
	mov eax, 1
	int 80h
	ret
