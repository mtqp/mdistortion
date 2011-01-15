%include "m_macros.asm"
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
section .data
	cte_dos : dd 2.0 

section .text
	global hall_asm

hall_asm:
	%define md_ptr 	[ebp-20]		;PORQUE!!?!?!?!?!?!?!?!?!?!?!?!?!
	%define i 		[ebp-16]		;hiper WTF
	
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

dummy_asm:
	ret

