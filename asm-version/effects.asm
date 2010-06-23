;float hall_func(m_distortion *md, float smp, int i){
;	int j;
;	float save_smp;
;	for(j=0;j<md->_hall->hll_buf_quantity;j++){
;		save_smp = smp;
;		smp += md->_hall->hll_bufs[j][i];
;		md->_hall->hll_bufs[j][i] = md->_hall->hll_coef*(save_smp/2);
;	}
;	return smp;
;} tengo q hacerlo para cuatro samples!

hall:
	%define md_ptr 	[ebp+8]
	;%define smps   	[ebp+12]
	%define i 		[ebp+12]

	convencion_C
	
	mov 	edx,md_ptr 
	lea 	edx,[edx+28]	;ecx = ptr_hall
	
	mov 	ecx,[edx+8]		;edx = buf_quantity
	
	pxor    xmm5,xmm5
	pxor    xmm6,xmm6
	movss   xmm5,[ecx+4]	;xmm5=[0,0,0,coef]
	movlhps xmm5,xmm5		;xmm5=[0,coef,0,coef]
	movdqu  xmm6,xmm5		;xmm6=[0,coef,0,coef]
	pslldq  xmm6,4			;xmm6=[coef,0,coef,0]
	addps   xmm5,xmm6		;xmm5=[coef,coef,coef,coef]
	
	mov 	edx,[edx+12]	;edx = hall_bufs**
	xor 	eax,eax			;eax = j

ciclo_hl:
	movdqu 	xmm1,xmm0		;xmm1 = save_smp
	;divps	xmm1,2			;fijarse como subir un dos ahi...

	mov 	ebx,edx			;hall_bufs
	mov 	esi,eax			;j
	shl		esi,2			;convierto a j en indentacion de float*
	add		ebx,esi			;apunto al buffer correspondiente
	mov 	esi,i
	shl		esi,2			;convierto en i en indentacion de float*
	add 	ebx,esi
	
	movdqu	xmm3,[ebx]		;xmm3 = hall_bufs[j][i]
	addps  	xmm0,xmm3		;xmm0 = (smp += md->_hall->hll_bufs[j][i])

	mulps	xmm1,xmm3		;xmm1 = md->_hall->hll_coef*(save_smp/2);
	movdqu  [ebx],xmm1		;guardo en memoria

	add		eax,1			;habia otra q era com oaumentar uno estaba mas buena
	cmp		eax,ecx
	jne		ciclo_hl	
	convencion_C_fin

