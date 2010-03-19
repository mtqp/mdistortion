section .text:
	;escribirlo  bien
	mov eax, ecx
	mov ecx, 4
	div ecx	;ofjdofjgdofjg


	div ecx,4 	;esto es mas optimo q decrementar de a 4 a ecx
	mov ecx,eax
loop:
	emms		;pone todos los registros de mmx en cero
	movq mm0,[esi]
	movq mm1,[edi]
	pmaddwd mm0,mm1
	paddd mm7,mm0
	add esi,8
	add edi,8
	dec ecx
	jnz loop		
;de registros mmx a proposito gnal es muy costoso!
