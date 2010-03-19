global suma

section .text:

suma:
	push ebp
	mov ebp,esp
	push esi
	push edi
	push ebx
	mov ebx,[ebp+8]		;parte baja a
	mov ecx,[ebp+12]	;parte alta a
	mov esi,[ebp+16]	;parte baja b
	mov edi,[ebp+20]	;parte alta b
	add ebx,esi		;sumo parte baja
	adc ecx,edi	;sumo parte alta con el carry q pueda haber
	mov eax,ebx		;la muevo para printear
	mov edx,ecx 	;muevo la alta tbm para printear

	pop ebx
	pop edi
	pop esi
	pop ebp
	ret
