;// int strcmp( const char *str1, const char *str2 );
;// size_t strlen( char *str );
extern strcmp
extern malloc

section .data
	fuck : db "d"
section .text
	global agregarPalabra
	
agregarPalabra:
	%define null 0
	%define ptr_dicc [ebp+8]
	%define ptr_char [ebp+12]
	%define ptr_sig  [ebp+16]
	
	push ebp
	mov ebp,esp
	push esi
	push edi
	push ebx

	mov esi,ptr_dicc	;apunta a dicc_arb_bin
	mov esi,[esi]		;es nodo* raiz!
	cmp esi,null
	je agregarRAIZ
	
	mov edi,ptr_char
	;mov eax,[eax]	;estoy parado en el 1er nodo

buscar_donde_agregar:
	;cmp edi,null
	;je agregarNODO
	push edi
	push esi
	call strcmp
	lea esp,[esp+8]
	cmp eax,0		;elijo el lado a recorrer del arbol
	jl agregarIZQ

agregarDER:
	lea esi,[esi+16]		;esi == dicc.raiz->der no seee
	cmp dword [esi],null
	je agregarNODO
	mov esi,[esi]	;me meto en el nuevo nodo
	jmp buscar_donde_agregar

agregarIZQ:
	lea esi,[esi+12]		;hijo izq
	cmp dword [esi],null
	je agregarNODO
	mov esi,[esi]
	jmp buscar_donde_agregar

agregarNODO:
	mov ebx,16		;pido memoria para el nodo
	push ebx
	call malloc
	add esp,4
	
	mov [esi],eax	;guardo en el hijo el nuevo nodo
	;mov ebx,ptr_char
	mov ebx,fuck
	;mov ecx,ptr_sig
	mov ecx,fuck
	mov [eax],ebx
	mov [eax+4],ecx
	mov dword [eax+8],null
	mov dword [eax+12],null
	jmp fin_agregarP
	
agregarRAIZ:
	mov ebx,16
	push ebx
	call malloc
	add esp,4
	
	mov ebx,ptr_dicc
	mov [ebx],eax		;asigno como raiz al nuevo nodo creado
	mov esi,ptr_char
	mov [eax],esi
	mov esi,ptr_sig
	mov [eax+4],esi
	mov dword [eax+8],null
	mov dword [eax+12],null

fin_agregarP:
	pop ebx
	pop edi
	pop esi
	pop ebp
	ret
