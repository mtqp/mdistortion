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
eq_asm2:			;fijarse si se puede evitar el pushad
	%define eq_ptr [ebp-20]
	pushad					;xmm0 = [smp0,smp1,smp2,smp3]
	;convencion_C	
	
	;	eq_sample = eq->a0[0] * sample + eq->a1 * eq->x1 + eq->a2 * eq->x2 - eq->a3 * eq->y1 - eq->a4 * eq->y2;
	
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
	
	movss	xmm5,xmm1		;xmm5 = [a0*smp0,0,0,0]
	addps	xmm3,xmm5		;xmm3 = [eq_smp[0],0,0,0]
	movss	xmm0,xmm3		;xmm0 = [eqsmp[0],smp1,smp2,smp3]

	movdqu	xmm5,[eax+32]	;xmm5 = [x1,x2,y1,y2]
	
	pslldq	xmm5,4			;xmm5 = [0,x1,x2,y1]
	pshufd	xmm6,xmm5,00100000b

	pxor	xmm5,xmm6		;xmm5 = [0,x1,0,y1]
	
	movdqu	xmm7,xmm3		;xmm7 = [eqsmp[0],0,0,0]	
	pslldq	xmm7,8			;xmm7 = [0,0,eq_sqmp[0],0]

	movss	xmm7,xmm0
	por 	xmm5,xmm7		;xmm5 = [smp,x1,eq_smp[0],y1] = [x1',x2',y1',y2']
	
	;falta hacer el return eqsmp!
	
;si se puede se puede obviar esta instruccion y mandarla solo al final
;	movdqu	[eax+32],xmm5
	movdqu	xmm3,xmm5		;xmm3 = [x1',x2',y1',y2']
	mulps	xmm3,xmm2
	mulps	xmm3,xmm4		;xmm3 = [x1'*a1,x2'*a2,-y1'*a3,-y2'*a4]

	haddps	xmm3,xmm3		;xmm3 = [x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4),x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4)]
	psrldq	xmm3,8			;xmm3 = [x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4),0,0]
	haddps	xmm3,xmm3		;xmm3 = [x1'*a1+x2'*a2+(-y1'*a3)+(-y2'*a4),0,0,0]
	
	movdqu	xmm7,xmm1		;xmm7 = [a0*smp0,a0*smp1,a0*smp2,a0*smp3] (puede ser una instruccion q mueva la quad baja)
	psrldq	xmm7,4			;xmm7 = [a0*smp1,a0*smp2,a0*smp3,0]
	pxor 	xmm6,xmm6
	movss	xmm6,xmm7		;xmm6 = [a0*smp1,0,0,0]
							
	addps	xmm3,xmm6 		;xmm3 = [eqsmp[1],0,0,0]
	pslldq	xmm3,4			;xmm3 = [0,eqmp[1],0,0]
	movss	xmm3,xmm0		;xmm3 = [eqsmp[0],eqsmp[1],0,0]
	movsd	xmm0,xmm3		;xmm0 = [eqsmp[0],eqsmp[1],smp2,smp3]
	
	pslldq	xmm5,4			;xmm5 = [0,x1',x2',y1']
	pxor	xmm7,xmm7
	pshufd	xmm7,xmm3,00100000b
	
	pxor	xmm5,xmm7		;xmm5 = [0,x1',0,y1']
	
	pslldq	xmm3,8			;xmm3 = [0,0,eqsmp[1],0]
	
	movdqu	xmm7,xmm0		;xmm7 = [smp0,smp1,smp2,smp3]
	psrldq	xmm7,4			;xmm7 = [smp1,smp2,smp3,0]
	movss	xmm3,xmm7		;xmm3 = [smp1,0,eqsmp[1],0]
	
	por		xmm5,xmm3		;xmm5 = [x1'',x2'',y1'',y2'']
	movdqu	xmm3,xmm5
	
;xmm0 --> smps!
;xmm1 --> a0*smp0,a0*smp1,a0*smp2,a0*smp3
;xmm2 --> [a1,a2,a3,a4]
;xmm3 --> x1'',x2'',y1'',y2''
;xmm4 --> 1,1,-1,-1
;xmm5,xmm6,xmm7 utilizables!

	mulps	xmm3,xmm2
	mulps	xmm3,xmm4		;xmm3 = [x1''*a1,x2''*a2,-y1''*a3,-y2''*a4]
	
	haddps	xmm3,xmm3		;xmm3 = [x1''*a1+x2''*a2,-y1''*a3+(-y2''*a4),x1''*a1+x2''*a2,-y1''*a3+(-y2''*a4)]
	psrldq	xmm3,8			;xmm3 = [x1''*a1+x2''*a2,-y1''*a3+(-y2''*a4),0,0]
	haddps	xmm3,xmm3		;xmm3 = [x1''*a1+x2''*a2+(-y1''*a3)+(-y2''*a4),0,0,0]
	
	movhlps	xmm7,xmm1		;xmm7 = [a0*smp2,a0*smp3,fruit,fruit]
	mulps	xmm3,xmm7		;xmm3 = [eqsmp[2],0,0,0]
	
	pxor	xmm5,xmm5
	movhlps	xmm5,xmm0		;xmm5 = [smp2,smp3,0,0]
	movlhps	xmm7,xmm0		;xmm7 = [0,0,smp2,smp3]
	pslldq 	xmm7,4
	psrldq	xmm7,4			;xmm7 = [0,0,smp2,0]
	pshufd	xmm7,xmm7,10010011b
							;xmm7 = [smp2,0,0,0]
	psrldq	xmm5,4
	pslldq	xmm5,4			;xmm5 = [0,smp3,0,0]
	movss	xmm6,xmm3

	pslldq	xmm3,8			;xmm3 = [0,0,eqsmp[2],0]
	por		xmm3,xmm7		;xmm3 = smp2,0,eqsmp[2],0]

	por		xmm6,xmm5		;xmm6 = [eqsmp[2],smp3,0,0]
	movlhps	xmm0,xmm6		;xmm0 = [eqsmp[0],eqsmp[1],eqsmp[2],smp3]
	

	
		
;/* Recalcular history buffers */
;	eq->x1 = sample;
;	eq->x2 = eq->x1;
;	eq->y1 = eq_sample;
;	eq->y2 = eq->y1;



	;convencion_C_fin	
	popad
	ret


eq_asm:			;hay registros sin utilizarse, todavia se puede mejorar mas!
	pushad					;xmm0 = [smp0,smp1,smp2,smp3]
	;convencion_C			;xmm1 = [a0*smp0,a0*smp1,a0*smp2,a0*smp3]
							;xmm2 = [x1,x2,y1,y2]
							;el resto son usables
	
	mov 	eax,eq_ptr
	movdqu	xmm1,[eax]		;xmm1 = [a0,a0,a0,a0]
	mulps	xmm1,xmm0		;xmm1 = [a0*smp0,a0*smp1,a0*smp2,a0*smp3]
	movdqu	xmm2,[eax+32]	;xmm2 = [x1,x2,y1,y2]


	movdqu	xmm3,[eax+16]	;xmm3 = [a1,a2,a3,a4]
	mov		ebx,mask_eq
	movdqu	xmm4,[ebx]		;xmm4 = [1,1,-1,-1]

	mulps	xmm3,xmm2		;xmm3 = [a1*x1,a2*x2,a3*y1,a4*y2]
	mulps	xmm3,xmm4		;xmm3 = [a1*x1,a2*x2,-a3*y1,-a4*y2]
		;pisable xmm4
	
	haddps	xmm3,xmm3		;xmm3 = [x1*a1+x2*a2,-y1*a3+(-y2*a4),x1*a1+x2*a2,-y1*a3+(-y2*a4)]
	psrldq	xmm3,8			;xmm3 = [x1*a1+x2*a2,-y1*a3+(-y2*a4),0,0]
	haddps	xmm3,xmm3		;xmm3 = [x1*a1+x2*a2+(-y1*a3)+(-y2*a4),0,0,0] -->ojo q en [1] deja fruta!!!!! va a explotar todo asi
;esto lo soluciona
	movlhps	xmm3,xmm3
	pslldq	xmm3,4
	psrldq	xmm3,4
	movhlps	xmm3,xmm3
	pxor	xmm7,xmm7
	movlhps	xmm3,xmm7
;esto lo soluciono
	
	pxor	xmm4,xmm4
	movss	xmm4,xmm1		;xmm4 = [ao*smp,0,0,0]
	addps	xmm3,xmm4		;xmm3 = [eqsmp[0],0,0,0]
	
		;pisable xmm4
	movss	xmm4,xmm0		;xmm4 = [smp,0,0,0]
	movlhps xmm4,xmm3		;xmm4 = [smp,0,eqsmp[0],0]
		;pisable xmm3
	pxor 	xmm7,xmm7
	movhlps	xmm7,xmm4
	movss	xmm0,xmm7
	
;	movss	xmm0,xmm4		;xmm0 = [eqsmp0,smp1,smp2,smp3]
		
	pslldq	xmm2,4			;xmm2 = [0,x1,x2,y1]
	movhlps	xmm3,xmm2		;xmm3 = [x2,y1,0,0]
	psrldq	xmm3,4
	pslldq	xmm3,4			;xmm3 = [0,y1,0,0]
	movlhps	xmm2,xmm3		;xmm2 = [0,x1,0,y1]
		;pisable xmm3
	por		xmm2,xmm4		;xmm2 = [x1',x2',y1',y2']
		;pisable xmm4
;------
;------
eqb:
	movdqu	xmm3,[eax+16]	;xmm3 = [a1,a2,a3,a4]
	mov		ebx,mask_eq
	movdqu	xmm4,[ebx]		;xmm4 = [1,1,-1,-1]

	mulps	xmm3,xmm2		;xmm3 = [a1*x1',a2*x2',a3*y1',a4*y2']
	mulps	xmm3,xmm4		;xmm3 = [a1*x1',a2*x2',-a3*y1',-a4*y2']
		;pisable xmm4

	haddps	xmm3,xmm3		;xmm3 = [x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4),x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4)]
	psrldq	xmm3,8			;xmm3 = [x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4),0,0]
	haddps	xmm3,xmm3		;xmm3 = [x1'*a1+x2'*a2+(-y1'*a3)+(-y2'*a4),0,0,0] -->ojo q en [1] deja fruta!!!!! va a explotar todo asi
;esto lo soluciona
	movlhps	xmm3,xmm3
	pslldq	xmm3,4
	psrldq	xmm3,4
	movhlps	xmm3,xmm3
	pxor	xmm7,xmm7
	movlhps	xmm3,xmm7
;esto lo soluciono
	
	pxor	xmm4,xmm4
	movsd	xmm4,xmm1
	psrldq	xmm4,4			;xmm4 = [a0*smp2,0,0,0]
	addps	xmm3,xmm4		;xmm3 = [eqsmp2,0,0,0]
		;pisable xmm4
shit:
	movsd	xmm4,xmm0		;xmm4 = [eqsmp1,smp2,0,0]
	psrldq	xmm4,4
	movlhps xmm4,xmm3		;xmm4 = [smp,0,eqsmp2,0]
		;pisable xmm3
	pslldq	xmm3,4			;xmm3 = [0,eqsmp2,0,0]
	movlhps	xmm5,xmm0		;xmm5 = [X,X,eqsmp1,smp2]
	movhlps	xmm5,xmm5		;xmm5 = [eqsmp1,smp2,eqsmp1,smp2]
	movss	xmm3,xmm5		;xmm3 = [eqsmp1,eqsmp2,0,0]
	movsd	xmm0,xmm3		;xmm0 = [eqsmp1,eqsmp2,smp3,smp4]
		;pisable xmm3,xmm5
	pslldq	xmm2,4			;xmm2 = [0,x1',x2',y1']
	movhlps	xmm3,xmm2		;xmm3 = [x2',y1',0,0]
	psrldq	xmm3,4
	pslldq	xmm3,4			;xmm3 = [0,y1',0,0]
	movlhps	xmm2,xmm3		;xmm2 = [0,x1',0,y1']
		;pisable xmm3
	por		xmm2,xmm4		;xmm2 = [x1'',x2'',y1'',y2'']
		;pisable xmm4
;------	
;------
eqc:
	movdqu	xmm3,[eax+16]	;xmm3 = [a1,a2,a3,a4]
	mov		ebx,mask_eq
	movdqu	xmm4,[ebx]		;xmm4 = [1,1,-1,-1]

	mulps	xmm3,xmm2		;xmm3 = [a1*x1'',a2*x2'',a3*y1'',a4*y2'']
	mulps	xmm3,xmm4		;xmm3 = [a1*x1'',a2*x2'',-a3*y1'',-a4*y2'']
		;pisable xmm4	

	haddps	xmm3,xmm3		;xmm3 = [x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4),x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4)]
	psrldq	xmm3,8			;xmm3 = [x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4),0,0]
	haddps	xmm3,xmm3		;xmm3 = [x1'*a1+x2'*a2+(-y1'*a3)+(-y2'*a4),0,0,0] -->ojo q en [1] deja fruta!!!!! va a explotar todo asi
;esto lo soluciona
	movlhps	xmm3,xmm3
	pslldq	xmm3,4
	psrldq	xmm3,4
	movhlps	xmm3,xmm3
	pxor	xmm7,xmm7
	movlhps	xmm3,xmm7
;esto lo soluciono

	pxor	xmm4,xmm4
	movhlps	xmm4,xmm1		;xmm4 = [ao*smp2,a0*smp3,0,0]
	movlhps	xmm4,xmm4
	pslldq	xmm4,4
	psrldq	xmm4,4
	movhlps	xmm4,xmm4		;xmm4 = [a0*smp2,0,a0*smp2,0]
	pxor 	xmm7,xmm7		
	movlhps	xmm4,xmm7		;xmm4 = [a0*smp2,0,0,0]
	addps	xmm3,xmm4		;xmm3 = [eqsmp2,0,0,0]
		;pisable xmm4
		
	movhlps	xmm4,xmm0		;xmm4 = [smp2,smp3,0,0]
	movlhps	xmm4,xmm4		;xmm4 = [smp2,smp3,smp2,smp3]
	psrldq	xmm4,4
	pslldq 	xmm4,4			;xmm4 = [0,smp3,smp2,smp3]
	
	por		xmm4,xmm3		;xmm4 = [eqsmp2,smp3,smp2,smp3]
	movlhps	xmm0,xmm4		;xmm0 = [eqsmp0,eqsmp1,eqsmp2,smp3]
	
	movlhps	xmm3,xmm3		;xmm3 = [eqsmp2,0,eqsmp2,0]
	psrldq	xmm3,4
	pslldq	xmm3,4			;xmm3 = [0,0,eqsmp2,0]
	
	pslldq	xmm4,4
	psrldq	xmm4,4
	movhlps	xmm4,xmm4		;xmm4 = [smp2,0,smp2,0]
	
	movlhps	xmm4,xmm7		;xmm4 = [smp2,0,0,0]
	
	por 	xmm4,xmm3		;xmm4 = [smp2,0,eqsmp2,0]
		;pisable xmm3
	pslldq	xmm2,4			;xmm2 = [0,x1'',x2'',y1'']
	movhlps	xmm3,xmm2		;xmm3 = [x2'',y1'',fruit,0]
	psrldq	xmm3,4
	pslldq	xmm3,4			;xmm3 = [0,y1'',fruit,0]
	movlhps	xmm2,xmm3		;xmm2 = [0,x1'',0,y1'']
		;pisable xmm3
	por		xmm2,xmm4		;xmm2 = [x1''',x2''',y1''',y2''']
;------
;------
eqd:
	movdqu	xmm3,[eax+16]	;xmm3 = [a1,a2,a3,a4]
	mov		ebx,mask_eq
	movdqu	xmm4,[ebx]		;xmm4 = [1,1,-1,-1]

	mulps	xmm3,xmm2		;xmm3 = [a1*x1''',a2*x2''',a3*y1''',a4*y2''']
	mulps	xmm3,xmm4		;xmm3 = [a1*x1''',a2*x2''',-a3*y1''',-a4*y2''']
		;pisable xmm4	
	
	haddps	xmm3,xmm3		;xmm3 = [x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4),x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4)]
	psrldq	xmm3,8			;xmm3 = [x1'*a1+x2'*a2,-y1'*a3+(-y2'*a4),0,0]
	haddps	xmm3,xmm3		;xmm3 = [x1'*a1+x2'*a2+(-y1'*a3)+(-y2'*a4),0,0,0] -->ojo q en [1] deja fruta!!!!! va a explotar todo asi
;esto lo soluciona
	movlhps	xmm3,xmm3
	pslldq	xmm3,4
	psrldq	xmm3,4
	movhlps	xmm3,xmm3
	pxor	xmm7,xmm7
	movlhps	xmm3,xmm7
;esto lo soluciono

	pxor	xmm4,xmm4
	movhlps	xmm4,xmm1		;xmm4 = [ao*smp2,a0*smp3,0,0]
	psrldq	xmm4,4			;xmm4 = [a0*smp3,0,0,0]
	addps	xmm3,xmm4		;xmm3 = [eqsmp3,0,0,0]

		;pisable xmm4
		
	movhlps	xmm4,xmm0		;xmm4 = [eqsmp2,smp3,0,0]
	movlhps	xmm4,xmm4		;xmm4 = [eqsmp2,smp3,eqsmp2,smp3]
	movdqu	xmm6,xmm4		;xmm6 = [eqsmp2,smp3,eqsmp2,smp3]
	pslldq	xmm4,4
	psrldq	xmm4,4			;xmm4 = [eqsmp2,smp3,eqsmp2,0]
	movhlps	xmm4,xmm4		;xmm4 = [eqsmp2,0,eqsmp2,0]
	
	pslldq	xmm3,4			;xmm3 = [0,eqsmp3,0,0]
	
	por		xmm4,xmm3		;xmm4 = [eqsmp2,eqsmp3,eqsmp2,0]
	movlhps	xmm0,xmm4		;xmm0 = [eqsmp0,eqsmp1,eqsmp2,eqsmp3]	=D
	
	pslldq	xmm3,4			;xmm3 = [0,0,eqsmp3,0]

fuck:
	psrldq  xmm6,4
	movhlps	xmm4,xmm6		;xmm4 = [smp3,0,0,0]
	pxor	xmm7,xmm7
	movlhps	xmm4,xmm7
	por		xmm4,xmm3		;xmm4 = [smp3,0,eqsmp3,0]
			;pisable xmm3
	pslldq	xmm2,4			;xmm2 = [0,x1''',x2'',y1''']
	pxor	xmm3,xmm3
	movhlps	xmm3,xmm2		;xmm3 = [x2''',y1''',0,0]
	psrldq	xmm3,4
	pslldq	xmm3,4			;xmm3 = [0,y1''',0,0]
	movlhps	xmm2,xmm3		;xmm2 = [0,x1''',0,y1''']
		;pisable xmm3
	por		xmm2,xmm4		;xmm2 = [x1'''',x2'''',y1'''',y2'''']

	movdqu	[eax+32],xmm2	;guardo los factores de vuelta en mem

	;convencion_C_fin	
	popad
	ret
	

