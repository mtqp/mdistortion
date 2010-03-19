%include "macros.asm"

section .data

section .text
	global recorrer_nodos_final
	global seleccion_array_subi
		
;me mandan un puntero a nodo!
recorrer_nodos_final:
	%define ptr_nodo [ebp+8]
	%define null 0
	convencion_C
	mov esi,ptr_nodo

	cmp dword [esi+4],null		;caso un nodo
	je fin_rec
ciclo_rec:				;caso varios nodos
	mov esi,[esi+4]
	cmp dword [esi+4], null
	jne ciclo_rec
fin_rec:
	mov eax,esi
	convencion_C_fin
	ret
		
;me pasan un numero entre -1000 y 1000
seleccion_array_subi:
	convencion_C
	%define numero [ebp+8]
	mov esi,numero
	cmp esi,0
	jl numero_neg
numero_pos:
	mov eax,10		;res
	mov edi,0		;contador!
ciclo_pos:
	sub esi,100
	cmp esi,0
	jle fin_selec		;puse mal el ultimo hash 900/1000
	inc edi			;sino seria jl solo.
	jmp ciclo_pos
numero_neg:
	mov eax,0
	mov edi,9
ciclo_neg:
	add esi,100
	cmp esi,0
	jge fin_selec
	dec edi
	jmp ciclo_neg
fin_selec:
	add eax,edi
	convencion_C_fin
	ret

