%include "macros.asm"
extern free
extern malloc
extern printf

os_dato equ 0
os_ptr_sig equ 4

section .data
	error_buscar: DB "elemento inexistente",10
	error_sz: equ $-error_buscar
	error_elim_pf: DB "elemento inexistente,no puede eliminarse",10
section .text
	global inicializarL
	global agregarAD
	global agregarAT
	;global agregarORD
	global iesimo
	global buscar
	global eliminarN		;borrar 1ra aparicion de N
	;global eliminarAPAR
	;global eliminarREP
	;global eliminarL
	;global agregarORDL
			
eliminarN:
	%define ptr_lista [ebp+8]
	%define dato [ebp+12]
	%define null 0
	
	push ebp
	mov ebp,esp
	push esi
	push edi
	push ebx			;convencion c
	
	mov ebx,ptr_lista
	mov ecx,dato
	mov ebx,[ebx]			;me manejo desde un principio con el
					;*prim
	cmp ebx,null
	je error_elim
					;caso lista un elem
	cmp dword [ebx+4],null
	jne ciclo_elim
elim_unario:
	mov eax,ebx			;es el dato q quiero eliminar?
	cmp dword [eax],ecx
	jne error_elim
	push ebx
	call free
	add esp,4
	mov eax,ptr_lista		;el usado solo para mod *lista
	mov dword [eax],null		;*prim = NULL
	mov dword [eax+4],null		;*ult = NULL
	jmp fin_elim
ciclo_elim:
	mov ecx,[ebx]			;tuve q agregar x invalid op & opcode
	cmp ecx,dato
	je eliminar_nodo
	mov ebx,[ebx+4]
	cmp ebx,null
	je error_elim
	jmp ciclo_elim
eliminar_nodo:
	mov esi,ebx			;no se si el free me destruye ebx
	mov edi,[ebx+4]			;siguiente al eliminado
	cmp edi,null
	je elim_ult_nodo
	push ebx
	call free
	add esp,4
	mov [esi],edi			;restauro ptrs
	jmp fin_elim
elim_ult_nodo:
	mov dword [eax+4],ebx		;en la lista modifico cual es el *ult
	push ebx
	call free
	add esp,4
	mov dword [esi],null			;xq es el ultimo nodo
	jmp fin_elim
error_elim:
	push error_elim_pf
	call printf
	add esp,4
fin_elim:
	pop ebx				;convencion c fin
	pop edi
	pop esi
	pop ebp
	ret
			
buscar:
	%define dato_encontrar [ebp+12]
	%define prt_l [ebp+8]
	convencion_C
	mov ebx,prt_l
	mov ecx,dato_encontrar
	mov eax,0			;contador iesimo res

	mov ebx,[ebx]			;me meto en la lista
	cmp ebx,0			;caso vacia
	je error_b

ciclo_b:
	cmp [ebx],ecx			;comparo con dato
	je fin_b
	cmp dword [ebx+4],0		;si paso el je fin_b
	je error_b			;me fijo si tengo otro nodo 4 check
	inc eax
	mov ebx,[ebx+4]
	jmp ciclo_b
error_b:
	push error_buscar
	call printf
	add esp,4
	mov eax,0xFFFFFFFF
fin_b:
	convencion_C_fin
	ret
			
agregarAT:
	%define ptr_l [ebp+8]
	%define dato [ebp+12]
	%define null 0
	%define tam_nodo 8
	convencion_C
	push tam_nodo
	call malloc
	add esp,4		;en eax esta el ptr_new nodo
	mov edi,dato		;lo necesito en reg para hacer el mov
	mov edx,ptr_l
	mov ecx,[edx]		;apunta el 1ro a null? offset cero
	jecxz l_vacia_at		;lista vacia
	jmp agregar_new_nodo_at
l_vacia_at:
	lea esi,[edx+4]
	mov [esi],eax		;ultimo apunta a  nodo
	mov [edx],eax		;primero apunta a nodo

	mov dword [eax+os_dato],edi
	mov dword [eax+os_ptr_sig],null	;seteo de NODO

	jmp finAT
agregar_new_nodo_at:
	lea edx,[edx+4]		;ptr a ex_ultimo
	mov edx,[edx]
	lea edx,[edx+4]		;[[edx]+4] apunto a nuevo ult
	mov [edx],eax
	
	mov dword [eax+os_dato],edi
	mov dword [eax+os_ptr_sig],null
	
finAT:
	convencion_C_fin
	ret
	
inicializarL:
	%define ptr_l [ebp+8]
	convencion_C
	mov eax,ptr_l
	mov dword [eax],0
	mov dword [eax+4],0
	convencion_C_fin
	ret
	
iesimo:
	%define ptr_lista [ebp+8]
	%define iesimo [ebp+12]
	offset_sig equ 4
	convencion_C
	mov edx,iesimo		;iesimo a buscar
	mov ecx,ptr_lista
	mov ecx,[ecx]		;tengo ptr_prim
				;caso piden el primer elemento
	cmp edx,0
	je fin_iesimo
ciclo_iesimo:			;recorre la lista hasta encontrarlo
	mov ecx,[ecx+offset_sig]
	dec edx
	cmp edx,0
	je fin_iesimo
	jmp ciclo_iesimo
fin_iesimo:
	mov eax,[ecx]		;paso el dato a eax para devolverlo
	convencion_C_fin
	ret
	
agregarAD:
	%define ptr_lista [ebp+8]
	%define new_dato [ebp+12]
	%define null 0
	%define tam_nodo 8
	convencion_C
	push tam_nodo
	call malloc
	add esp,4		;en eax esta el ptr_new nodo
	mov edi,new_dato	;lo necesito en reg para hacer el mov
	;mov ecx,0
	mov ecx,ptr_lista
	mov ecx,[ecx]		;apunta el 1ro a null? offset cero
	jecxz l_vacia		;lista vacia
	jmp agregar_new_nodo

l_vacia:
	mov dword [eax+os_dato],edi
	mov dword [eax+os_ptr_sig],null
	xor esi,esi
	add esi,4		;apunto a esi al *ult de lista
	add esi,ptr_lista
	mov [esi],eax		;le paso el ptr_malloc
	jmp finAD
agregar_new_nodo:
	mov esi,ptr_lista
	mov esi,[esi]
	mov dword [eax+os_dato],edi	;x legibilidad dejo la linea repetida
	mov dword [eax+os_ptr_sig],esi
finAD:
	mov edx,ptr_lista		;ptr a la lista
	mov [edx],eax			;guardo en ptr al primero
	convencion_C_fin
	ret
