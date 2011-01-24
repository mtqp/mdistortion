%include "m_macros.asm"

	%define md_ptr 	[ebp-20]		;PORQUE!!?!?!?!?!?!?!?!?!?!?!?!?!
	%define i 		[ebp-16]		;hiper WTF

section .data
	cte_dos: dd 2.0 
	mask_eq: dd 1.0,1.0,-1.0,-1.0

section .text
	global hall_asm
	global delay_asm
	global eq_asm

;////////////////////////////////////////
hall_asm:
	;convencion_C
	pushad					;necesito hacer push de todos los registros, xq se modifica de todo! (dsp ver si se puede evitar)
	mov 	edx,md_ptr 
	mov 	edx,[edx+28]	;edx = ptr_hall
	
	mov 	ecx,[edx+8]
	
	upload_cte xmm5, xmm6, edx+4 	;xmm5=[coef,coef,coef,coef]
	
	mov 	edx,[edx+12]		 	;edx = hall_bufs**
	xor 	eax,eax					;eax = j

	upload_cte xmm4, xmm6, cte_dos	;xmm4=[2.0,2.0,2.0,2.0]
ciclo_hl:
	movdqu 	xmm1,xmm0				;xmm1 = save_smp
	divps	xmm1,xmm4				;xmm1=[save_smp/2,save_smp/2,save_smp/2,save_smp/2]

	mov 	ebx,edx			;ebx = hall_bufs**
	mov 	esi,eax			;j
	shl		esi,2			;convierto a j en indentacion de float*
	add		ebx,esi			;apunto al buffer correspondiente
	mov 	ebx,[ebx]		;ebx=hall[j]
	mov 	esi,i
	shl		esi,2			;convierto en i en indentacion de float*
	add 	ebx,esi
	
	movdqu	xmm3,[ebx]		;xmm3 = hall_bufs[j][i]
	addps  	xmm0,xmm3		;xmm0 = (smp += md->_hall->hll_bufs[j][i])

	mulps	xmm1,xmm3		;xmm1 = md->_hall->hll_coef*(save_smp/2);
	movdqu  [ebx],xmm1		;guardo en memoria

	inc 	eax			
	cmp		eax,ecx
	jne		ciclo_hl	
	
	popad				;restaura todos los registros
	ret
	;convencion_C_fin
;////////////////////////////////////////
dummy_asm:
	ret
;////////////////////////////////////////
delay_asm:	
	pushad
;	convencion_C
	
	mov 	esi,md_ptr
	mov 	edi,i
	mov		esi,[esi+24]	;edx = delay*
							;[esi+8] = cant_buf_active
	xor		eax,eax
	mov		edx,[esi+20]	;edx = float** dl_bufs	
ciclo_delay:
	shl		eax,2			;eax corrimiento floats
	mov		ecx,edx	
	mov		ecx,[ecx+eax]
	mov		ebx,[esi+4]
	shl		ebx,2
	add		ecx,ebx		;me posiciono en la matriz

	movdqu	xmm1,[ecx]		;xmm1 = matriz[i][j]	
	movdqu	xmm2,xmm0		;xmm2 = smp

	addps	xmm0,xmm1		;xmm0 += matriz[i][j]
	
	movdqu	[ecx],xmm2		;matriz[i][j] = smp

	shr		eax,2			;vuelvo para saber que cantidad itere
	inc		eax				;itero por todos los buffers
	cmp		eax,[esi+8]
	jl		ciclo_delay
	
;//////////	md->_delay->dl_sub_i++;///////////
	mov		eax,[esi+4]		;eax = dl_sub_i
	add		eax,4			;eax += 4

	cmp		eax,[esi+16]	;¿¿dl_subi == dl_speed??
	jne	fin_delay
	mov		eax,0			;reset dl_sub_i
	
fin_delay:
	mov	dword [esi+4],eax

;	convencion_C_fin
	popad
	ret
;////////////////////////////////////////
eq_asm:			;fijarse si se puede evitar el pushad
	%define eq_ptr [ebp-20]
	pushad					;xmm0 = [smp0,smp1,smp2,smp3]
	;convencion_C	
	
	mov 	eax,eq_ptr
	movdqu	xmm1,[eax]		;xmm1 = [a0,a0,a0,a0]
	mulps	xmm1,xmm0		;xmm1 = [a0*smp0,a0*smp1,a0*smp2,a0*smp3]
	
	movdqu	xmm2,[eax+16]	;xmm2 = [a1,a2,a3,a4]
	movdqu	xmm3,[eax+32]	;xmm3 = [x1,x2,y1,y2]
	mov		eax,mask_eq
	movdqu	xmm4,[eax]		;xmm4 = [1,1,-1,-1]
	
	mulps	xmm3,xmm2		;xmm3 = [x1*a1,x2*a2,y1*a3,y2*a4]
	mulps	xmm3,xmm4		;xmm3 = [x1*a1,x2*a2,-y1*a3,-y2*a4]
	
	haddps	xmm3,xmm3		;xmm3 = [x1*a1+x2*a2,-y1*a3+(-y2*a4),x1*a1+x2*a2,-y1*a3+(-y2*a4)]
	psrldq	xmm3,8			;xmm3 = [x1*a1+x2*a2,-y1*a3+(-y2*a4),0,0]
	haddps	xmm3,xmm3		;xmm3 = [x1*a1+x2*a2+(-y1*a3)+(-y2*a4),0,0,0]
	
	mulps	xmm3,xmm1		;xmm3 = [eq_smp[0],0,0,0]

	movdqu	xmm5,[eax+32]	;xmm5 = [x1,x2,y1,y2]
	;instruccion extra para ver si anda bien o no
	addps	xmm5,xmm0
	pslldq	xmm5,4			;xmm5 = [0,x1,x2,y1]
	pshufd	xmm6,xmm5,00100000b

	movdqu	xmm6,xmm5
	pslldq	xmm6,4			;xmm6 = [0,0,x1,x2]
	psrldq	xmm6,12			;xmm6 = [x2,0,0,0]
	pslldq	xmm6,12			;xmm6 = [0,0,x2,0]
	
	pxor	xmm5,xmm6		;xmm5 = [0,x1,0,x2]
		
;/* Recalcular history buffers */
;	eq->x2 = eq->x1;
;	eq->x1 = sample;
;	eq->y2 = eq->y1;
;	eq->y1 = eq_sample;

	
	;convencion_C_fin	
	popad
	ret
	
