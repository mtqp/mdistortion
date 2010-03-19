%macro convencion_C 0
	push ebp
	mov ebp,esp
	push esi		;ebp-4
	push edi		;ebp-8
	push ebx		;ebp-12
%endmacro

%macro convencion_C_fin 0
	pop ebx
	pop edi
	pop esi
	pop ebp
%endmacro
