;%macro convencion_C 0
	push ebp
	mov ebp,esp
	push edi
	push esi
	push ebx
;%endmacro

section .data

section .text
	global hell_sqr
	
hell_sqr:
	




;%macro convencion_C_fin 0
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret
;%endmacro

