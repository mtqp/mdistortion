;aqui se declaran extern las funciones que se usen

section .data
	hell_var: dd 0.15	;PROBLEMONES

section .text
	global volume_hell_sqr
	
volume_hell_sqr:
	%define buf_out [ebp+8]
	%define md_ptr  [ebp+12]
	%define nframes [ebp+16]
	
;	---------> convencion_C 
	push ebp
	mov ebp,esp
	push edi
	push esi
	push ebx

	xor ecx,ecx		;int i=0
	mov edx,nframes ;contador ciclo
	mov eax,buf_out	;buf_salida
	mov ebx,md_ptr	;estr
	
	pxor    xmm0,xmm0
	pxor 	xmm1,xmm1
	movss   xmm0,[ebx+12]	;[0,0,0,vol]
	movss	xmm1,[hell_var]	;[0,0,0,0.15]
	mulss 	xmm0,xmm1		;
	addss	xmm0,xmm1		;[0,0,0,resvol]
	movlhps xmm0,xmm0		;[0,resvol,0,resvol]
	movdqu  xmm1,xmm0		;[0,resvol,0,resvol]
	pslldq  xmm1,4			;[resvol,0,resvol,0]
	addps   xmm0,xmm1		;[resvol,resvol,resvol,resvol] (0.015+(0.015*mdc->_dvol))
	
	jmp $
;	-------> fin convencion C
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret

