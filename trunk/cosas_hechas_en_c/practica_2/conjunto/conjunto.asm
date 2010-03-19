extern malloc

section .data
	fuck : db "d"

section .text
	global agregarElemento
	global esta
	
;int esta(conj c, int elem);
esta:
	%define null 0
	%define ptr_conj 	[ebp+8]
	%define elem 		[ebp+12]
		
	push ebp
	mov ebp,esp
	push esi
	push edi
	push ebx

	xor eax,eax			;presuponemos q NO esta y devolvemos cero en ese caso
	mov edi,ptr_conj
	mov esi,elem
	
	mov eax,[edi]		;eax = *primero
	cmp eax,null
	je fin_esta			;(vacio?(conj))
	
ciclo_esta:
	cmp esi,[eax]		;if !(actual < elem || llegueALfin) sigoBuscando
	jl fin_esta			;else no lo encontre
	je encontro			; actual == elem
	cmp dword [eax+4],null
	je fin_esta
	mov eax,[eax+4]
	jmp ciclo_esta
	
encontro:
	mov eax,1
fin_esta:
	pop ebx
	pop edi
	pop esi
	pop ebp
	ret
	
;void agregarElemento(conj* c, int elem);
	
agregarElemento:
	%define null 0
	%define ptr_conj 	[ebp+8]
	%define elem 		[ebp+12]
		
	push ebp
	mov ebp,esp
	push esi
	push edi
	push ebx

	mov esi,elem
	
	mov eax,ptr_conj		;if (vacio(conj)) agregarPrimer_elem
	cmp dword [eax],null	;else buscar donde agregar
	je agregarPRIM
	
	mov edx,eax				;edx = puntero a LO ANTERIOR
	mov eax,[eax]			;eax = *(nodo)

	cmp dword [eax],esi		;ver si tengo q insertar como 1er elemento
	jg agregarAD
	jmp buscar_donde_agregar


buscar_donde_agregar:
	cmp esi,[eax]			;if (elem < nodo.dato) agregarAdelante
	jle agregarN				;else nodo = nodo.siguiente
	
	cmp dword [eax+4],null		;if (es ultimo elemento) agrego al fin
	je agregarALFin
	
	mov edx, [edx+4]		;este va siempre uno atras (siendo el ptr al nodo ult recorr)
	mov eax, [eax+4]		
	jmp buscar_donde_agregar

agregarAD:
	
	mov edi,eax				;
	mov esi,edx				;guardo ptr_a_ultimo
	mov eax,8
	push eax
	call malloc
	add esp,4				;eax = *newnodo
	
	mov [esi+4],eax			;
	mov ebx,elem
	mov [eax],ebx
	mov [eax+4],edi
	jmp fin_agregarElemento

agregarALFin:
							;eax = ptr(ultimoelem)
	mov edi,eax				;edi = eax
	mov eax,8
	push eax
	call malloc
	add esp,4				;eax = * newnodo
	
	mov [edi+4],eax			;nodoAnterior.siguiente = newnodo
	mov [eax],esi			;newnodo.dato = elem
	mov dword [eax+4],null
	jmp fin_agregarElemento

agregarPRIM:
							;eax = ptr(conj)
	mov edi,eax				;edi = eax = ptr(conj)
	mov eax,8
	push eax
	call malloc
	add esp,4				; eax = new(nodo) =)
	mov [edi],eax			;ptr(conj)->primero = eax (new[nodo])
	mov [eax],esi			;newnodo.elem = esi = elem
	mov dword [eax+4],null	;newnodo.siguiente = null
	jmp fin_agregarElemento
	
agregarN:
		
	;mov esi,edx				;esi = nodo(anterior)??
							;eax = ptr(anteriorAAgregar)
	mov edi,eax				;edi = eax
	mov eax,8
	push eax
	call malloc
	add esp,8				;eax = new(nodo)
	
	mov ecx,[edi+4]			; ecx = ptr(siguienteAAgregar)
	mov [edi+4],eax			; nodoAnterior.siguiente = newnodo
	mov esi,elem
	mov [eax],esi			; newnodo.elem = elem
	mov [eax+4],ecx    		; newnodo.siguiente = ptr(siguienteAAgregar)
							
fin_agregarElemento:
	pop ebx
	pop edi
	pop esi
	pop ebp
	ret
