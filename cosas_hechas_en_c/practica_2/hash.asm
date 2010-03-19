%include "macros.asm"

extern printf
extern malloc
extern free
extern recorrer_nodos_final		;debo pasarle el ptr_prim_nodo
extern seleccion_array_subi		;debo pasarle el numero q necesito

section .data
	mje_error_elim: DB "numero inexistente",10
section .text
	global estaH
	global agregarH
	global eliminarH
	;global promedioH
			
eliminarH:
	%define ptr_hs [ebp+8]
	%define dato_elim [ebp+12]
	%define null 0
	convencion_C
	mov eax,dato_elim
	push eax
	call seleccion_array_subi
	add esp,4
	mov esi,eax
	shl esi,2			;para usarlo para moverme en ad
	mov ebx,ptr_hs
	add ebx,esi			;apunto al ptr a nodo correcto p/buscar
	
	mov edi,dato_elim		;para comparar
	mov ebx,[ebx]
	cmp ebx,null			;lista nodos vacia
	je error_elim
	
	cmp dword [ebx+4],null
	je un_nodo
	
ciclo_elim:
	cmp [ebx],edi
	je elim_nodo
	cmp dword [ebx+4],null
	je error_elim
	lea ecx,[ebx]			;guardo ptr a anterior !!!!
	mov ebx,[ebx+4]			;sig ptr
	jmp ciclo_elim
un_nodo:
	cmp [ebx],edi			; [ebx]==dato a elim
	jne error_elim
	mov ecx,ebx
	push ebx
	call free			;borro el unico nodo q hay
	add esp,4
	mov eax,ptr_hs
	add eax,esi			;apunto al ptr*_i
	mov dword [eax],null
elim_nodo:
	mov edx,[ebx+4]
	mov [ecx+4],edx			;reapunto ptrs
	push ebx
	call free
	add esp,4
	jmp fin_elim
error_elim:
	push mje_error_elim
	call printf
	add esp,4
fin_elim:
	convencion_C_fin
	ret
			
estaH:
	%define ptr_hs [ebp+8]
	%define nro_buscar [ebp+12]
	%define null 0
	convencion_C
	mov eax,nro_buscar
	push eax
	call seleccion_array_subi
	add esp,4
	shl eax,2			;apunto a dword recordar!
	mov ebx,ptr_hs
	add ebx,eax			;desplazamiento calculado con s_a_subi

	mov ecx,nro_buscar

	cmp dword [ebx],null		;tiene algun nodo?
	je elem_no_existe
				;si pasa esta linea es xq algun nodo tiene
	mov ebx,[ebx]		;apunto al 1er nodo
	cmp [ebx],ecx		;nro_buscar == ptr->valor
	je elem_existe
ciclo_esta:
	mov ebx,[ebx+4]		;ebx = ptr->ptr_prox
	cmp [ebx],ecx		;ptr->valor == nro_buscar
	je elem_existe
	cmp dword [ebx+4],null	;ptr->prox == null == compare el ult elem?
	je elem_no_existe
	jmp ciclo_esta
elem_existe:
	mov eax,1		;true
	jmp fin_esta
elem_no_existe:
	mov eax,0		;false
fin_esta:
	convencion_C_fin
	ret
				
agregarH:
	%define ptr_hs [ebp+8]
	%define new_dato [ebp+12]
	%define null 0
	%define tam_nodo 8

	convencion_C
;veo primero a q nodo tiene q ir, recorro agrego y listo
;recorrer_nodos_final->ojo q tiene q tener un nodo/seleccion_array_subi
	mov esi,ptr_hs
	mov edi,new_dato
	
	push tam_nodo
	call malloc
	add esp,4
	
	mov ebx,eax		;ebx == ptr_new_nodo
	mov dword [ebx],edi	;seteo info en new_nodo
	mov dword [ebx+4],null
	
	push edi
	call seleccion_array_subi
	add esp,4
	shl eax,2		;corrimiento a int, para sumarselo al ptr_hs
	add esi,eax		;apunta al bucket correcto
	
	cmp dword [esi],null 		;bucket vacio o cargado?
	je bucket_vacio
	
	mov esi,[esi] 		;le paso *prim_nodo enBucket
	push esi
	call recorrer_nodos_final
	add esp,4 		;en eax tengo el ptro al ultimo nodo
	mov [eax+4],ebx 	;enlazo los ultimos 2 nodos
	jmp fin_agregarH
bucket_vacio:
	mov [esi],ebx
fin_agregarH:
	convencion_C_fin
	ret
