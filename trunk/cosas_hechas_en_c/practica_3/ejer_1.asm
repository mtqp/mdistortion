%include "macros.asm"

extern malloc
				
section .data
	mask_impar: DQ 0x0000FFFF0000FFFF
	mask_par: DQ 0xFFFF0000FFFF0000
	mask_chars_mul: DQ 0x8007800680058004
	mask_xmm_low: DQ 0x8003800280018000,0x8007800680058004
	mask_xmm_high: DQ 0x030A020901080007,0x070E060D050C040B
	mask_sum_x_i: DQ 0x0000FFFF0000FFFF
		;	MENOS SIGNIFIC		MAS SIGNIFIC

section .text
	global sumaYResta:
	global prodInterno:
	global prom:

				
prom:
	%define ptr_vector[ebp+8]
	%define prom [ebp+12]
	%define varianza [ebp+16]
	
	convencion_C
	mov edi,ptr_vector
	mov esi,8
	movq mm7,[mask_sum_x_i]
	pxor mm2,mm2
	pxor mm4,mm4
	
cicloPV:
	
	movq mm0,[edi]		;x_i
	movq mm1,mm0
	movq mm3,mm0		;x_i²
	pshufw mm0,mm0,11011000b
	pand mm0,mm7
	pshufw mm1,mm1,01100011b;en mascaro para crear dos nros de 32
	pand mm1,mm7
	paddd mm0,mm1
	
	paddd mm2,mm0		;acumulador de sumas parciales
	pmaddwd mm3,mm3
	paddd mm4,mm3		;acumulador de sumas de cudrados parciales
	
	add edi,8		;apunto a la nueva tira de bits
	sub esi,4		;decrementar contador
	cmp esi,0
	je finPV
	jmp cicloPV
finPV:				;aca debo hacer las operaciones pedidas
	pshufw mm1,mm2,10110001b
	paddd mm2,mm1
	pshufw mm3,mm4,10110001b	;hice mierda estas mascaras
	paddd mm4,mm3
				;prom(sum(x_i)
	xor edx,edx
	
	movd eax,mm2		;sum(x_i)
	mov ebx,8
	div ebx			;en eax tengo |sum(x_i)/n|
	mov ecx,prom
	mov [ecx],eax		;lo guardo en memoria
				;VARIANZA
	
	movd ecx,mm4		;sum(x_i²)
	mov eax,ecx		;sum(x_i²) / 256
	mov ebx,8
	div ebx			;quotiente en eax
	sub ecx,eax		;sum(x_i²) - [sum(x_i²)/256]
	mov eax,ecx
	div ebx			;en eax esta la VARIANZA
	mov ecx,varianza
	mov [ecx],eax
	convencion_C_fin
	ret
				
prodInterno:
	%define ptr_v1_sh [ebp+8]
	%define ptr_v2_char [ebp+12]
	%define tam_short [ebp+16]
	convencion_C
	xor eax,eax
	mov ax,tam_short
	mov edi,eax		;en edi me qdo con el tam para iterar
	shl eax,1		;los nros res ahora son de 32 bits
	push eax
	call malloc
	add esp,4		;reservo memoria para res.
	mov ebx,eax		;ptr_res en eax, iterador en ebx
	movq mm3,[mask_chars_mul]	;los deja en el lugar correcto
	mov ecx,ptr_v1_sh
	mov esi,ptr_v2_char	;hay q extenderlos a 16 bits
	movdqu xmm6,[mask_xmm_low]
	movdqu xmm7,[mask_xmm_high]
	
cicloPI:
	movq mm1,[ecx]		;tengo los packed de 16bits
	movq mm0,mm1		;necesito dos para el pmul
	movq mm2,[esi]		;tira de chars
	pshufb mm2,mm3		;me qdo con los chars q quiero para mul
	
	pmullw mm1,mm2		;tengo la parte baja
	pmulhw mm0,mm2		;tengo la parte alta
	movq2dq	xmm1,mm1	;parte baja de mul en xmm1
	movq2dq	xmm0,mm0	;parte alta del mul en xmm0
	pshufb xmm1,xmm6		;ordeno la parte baja de cada 32 bits
	pshufb xmm0,xmm7		;agrego cada parte alta de 32bits
	por xmm1,xmm0
	movdqu [ebx],xmm1	;lo guardo en memoria.
	add ebx,16		;guarde 4 nros de 32
	add ecx,8		;4 numeros de 16
	add esi,4		;4 chars de 8
	sub edi,4		;decremento contador
	cmp edi,0
	je finPI
	jmp cicloPI
	
finPI:
	convencion_C_fin
	ret
				
sumaYResta:
	%define ptr_v [ebp+8]
	%define ptr_w [ebp+12]
	%define tam [ebp+16]
	convencion_C
	xor eax,eax		;es short int tengo q levantar 16bits
	mov ax,tam		;preparo para pedir memoria
	;mov edx,eax		;tam sin shifteo
	shl eax,1		;multiplico x dos para malloquear tam en bytes
	push eax
	call malloc		;eax ptr_res
	add esp,4		;reserva memoria para res
	mov ebx,eax		;ptr_res donde itero
	movq mm6,[mask_impar]	;esperemos q funque el mov
	movq mm7,[mask_par]	;puede ser q necesite la U en vez de la A
	xor edx,edx
	mov dx,tam
	
ciclo:
	mov esi,ptr_v
	mov edi,ptr_w
	movq mm1,[esi]		;el par
	movq mm0,[edi]		;el par
	movq mm3,mm1		;el impar ptr_v
	movq mm2,mm0		;el impar ptr_w
	pand mm1,mm7		;filtro con las mascaras
	pand mm0,mm7
	pand mm3,mm6
	pand mm2,mm6		;ambos numeros con basura filtrada
	paddw mm1,mm0		;v0+w0
	psubw mm3,mm2		;v1-w1
	paddw mm1,mm3		;guardo res, la conjuncion de ambas oper
	movq [ebx],mm1		;guardo en res
	add dword ptr_v,8		;apunto a los 64 bits siguientes
	add dword ptr_w,8		;en V, W y RES
	add ebx,8
	sub edx,4		;estoy trabajando con 4 sub_i del vector
	cmp edx,0
	je fin
	jmp ciclo
	
fin:
	convencion_C_fin
	ret
		
