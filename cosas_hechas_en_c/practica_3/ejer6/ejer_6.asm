extern malloc
;section .data
	
;extern float* normalizar(int* v, unsigned short n);

section .text
	global normalizar
	
erosion:

	%define ptr_V 	[ebp+8]
	%define n	 	[ebp+12]
	define varQuad	[ebp-4]
	
	push ebp
	mov ebp,esp
	sub esp,8		[ebp-4]
	push esi
	push edi
	push ebx
	
	mov eax,ptr_A			;paso las variables a registros
	mov ebx,n
	
	pxor xmm7,xmm7			;xmm7 = acumulador
	
;/////XMM
ciclo_normalizar:
	mov xmm0,[eax]			;[i0,i1,i2,i3]
	pmaddwd xmm0,xmm0		;[(io²+i1²),(i2²+i3²)]
	paddq xmm7,xmm0			;xmm7 = resParcial
	
	add eax,16				;apunto a la nueva tira
	sub ebx,4
	cmp ebx,0				;¿termine?
	jne ciclo_normalizar
	
	movdqu xmm6,xmm7		;duplico [resParcial_0,resParcial_1]
	psrldq xmm6,8			;xmm6 = [resParcial_1,0]
	paddq xmm7,xmm6			;xmm7 = [resINT,0]
	
	movq varQuad,xmm7		;guardo para cargar en fpu
	
;/////FPU	
	finit
	fild qword varQuad		;[varQuad]
	
	fsqrt					;[resNORMALIZADO]

	fst dword varQuad
	
	pxor xmm0,xmm0
	movsd xmm0,varQuad
	movdqu xmm1,xmm1
	pslldq xmm1,4
	addps xmm0,xmm1
	pslldq xmm1,4
	addps xmm0,xmm1
	pslldq xmm1,4
	addps xmm0,xmm1			;xmm0 = [norma0,norma1,norma2,norma3]
	
	mov ebx,n

	push ebx
	call malloc
	add esp,4				;eax = float*

	mov edi,eax				
	mov esi,ptr_V
	
ciclo_floats:	
	movdqu xmm1,[esi]
	cvtdq2ps xmm1,xmm1
	divps xmm1,xmm0
	
	movdqu [edi],xmm1
	add edi,4
	add esi,4
	sub ebx,4
	cmp ebx,0
	jne ciclo_floats
							;eax = float*
fin_normalizar:
	pop ebx
	pop edi
	pop esi
	add esp,8
	pop ebp
	ret
