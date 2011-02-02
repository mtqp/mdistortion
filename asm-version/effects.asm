%include "m_macros.asm"

	%define md_ptr 	[ebp-20]	
	%define i 		[ebp-16]	

section .data
	cte_dos: dd 2.0 
	mask_eq: dd 1.0,1.0,-1.0,-1.0

section .text
	global dummy_func
	global hall_func
	global delay_func
	global equalizer_func

;////////////////////////////////////////
hall_func:
;precondicion  		-->	xmm0 =  [smpi,smpi+1,smpi+2,smpi+3]
;post				--> xmm0 = delay([smpi,smpi+1,smpi+2,smpi+3]) 
;registros usados	--> xmm1,xmm3,xmm4,xmm5,xmm6

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

	mulps	xmm1,xmm5		;xmm1 = md->_hall->hll_coef*(save_smp/2);
	movdqu  [ebx],xmm1		;guardo en memoria

	inc 	eax			
	cmp		eax,ecx
	jne		ciclo_hl	
	
	popad				;restaura todos los registros
	ret
	;convencion_C_fin
;////////////////////////////////////////
dummy_func:
	ret
;////////////////////////////////////////
delay_func:	
;precondicion  		-->	xmm0 =  [smpi,smpi+1,smpi+2,smpi+3]
;post				--> xmm0 = delay([smpi,smpi+1,smpi+2,smpi+3]) 
;registros usados	--> xmm1,xmm2
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
;precondicion 	--> xmm0 = [smpi,smpi+1,smpi+2,smpi+3]
;post			--> xmm0 = eq([smpi,smpi+1,smpi+2,smpi+3]) (bass,mid,treb)
;registros usados	--> xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7
equalizer_func:
	convencion_C
	;push 	ebx			;md_ptr!
	
	mov		esi,ebx
	mov		ebx,[esi+12]

;	push dword [ebx]
	call 	eq_asm		;bass
;	add 	esp,4

;	add		ebx,4
;	push dword [ebx]
	mov		ebx,[esi+16]
	call 	eq_asm		;treb
;	add 	esp,4

;	add 	ebx,4
;	push dword [ebx]
	mov		ebx,[esi+20]
	call 	eq_asm		;mid
;	add	 	esp,4

	;pop		ebx
	;ret
	convencion_C_fin
;////////////////////////////////////////


eq_asm:			;hay registros sin utilizarse, todavia se puede mejorar mas!
	;pushad					;xmm0 = [smp0,smp1,smp2,smp3]
	convencion_C			;xmm1 = [a0*smp0,a0*smp1,a0*smp2,a0*smp3]
							;xmm2 = [x1,x2,y1,y2]
							;el resto son usables
	
	mov 	esi,ebx
	movdqu	xmm1,[esi]		;xmm1 = [a0,a0,a0,a0]
	mulps	xmm1,xmm0		;xmm1 = [a0*smp0,a0*smp1,a0*smp2,a0*smp3]
	movdqu	xmm2,[esi+32]	;xmm2 = [x1,x2,y1,y2]


	movdqu	xmm3,[esi+16]	;xmm3 = [a1,a2,a3,a4]
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
	movdqu	xmm3,[esi+16]	;xmm3 = [a1,a2,a3,a4]
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
	movdqu	xmm3,[esi+16]	;xmm3 = [a1,a2,a3,a4]
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
	movdqu	xmm3,[esi+16]	;xmm3 = [a1,a2,a3,a4]
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

	movdqu	[esi+32],xmm2	;guardo los factores de vuelta en mem

	convencion_C_fin	
	;popad
	;ret
	

