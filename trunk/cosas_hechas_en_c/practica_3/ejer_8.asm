;section .data
	
section .text
	global multiplicarSIvalido
	
multiplicarSIvalido:

	%define ptr_v [ebp+8]
	%define ptr_w [ebp+12]
	%define short_n [ebp+16]

	push ebp
	mov ebp,esp
	push esi
	push edi
	push ebx
	
	mov eax,ptr_v
	mov ebx,ptr_w
	xor ecx,ecx
	mov cx,short_n
	
	pxor xmm7,xmm7
ciclo_mSIv:
	movdqu xmm0,[eax]	;tiene los datos
	movdqu xmm1,xmm0	;tiene la validacion
	movdqu xmm4,xmm1
	
	movdqu xmm2,[ebx]	;tiene los datos
	movdqu xmm3,xmm2	;tiene la validacion
	movdqu xmm5,xmm3
	
	psrldq xmm0,1		;[d1,-,d2,-,d3,-]
	psrldq xmm2,1
	
	punpcklbw xmm0,xmm7	;[d1,0,d2,0,d3,0]
	punpcklbw xmm2,xmm7	
	
	punpcklbw xmm1,xmm7	;[v1,0,v2,0,v3,0]
	punpcklbw xmm3,xmm7
	
	pand xmm0,xmm1		;[dv1,0,dv2,0,dv3,0]
	pand xmm2,xmm3
	
	pmullw xmm0,xmm2	;[r1,0,r2,0,r3,0]
	
	punpcklbw xmm0,xmm7	;[r1,0,r2,0,r3,0] (x las dudas si mul se pasa de byte)
	
	pslldq xmm0,1		;[0,r1,0,r2,0,r3]

	pand xmm1,xmm3		;[vr1,0,vr2,0,vr3,0]
	
	por xmm0,xmm1		;[vr1,r1,vr2,r2,vr3,r3]	
	
	movdqu [eax],xmm0
	
	add eax,16
	add ebx,16
	sub ecx,16
	cmp ecx,0
	je fin_mSIv
	jmp ciclo_mSIv
		
fin_mSIv:
	pop ebx
	pop edi
	pop esi
	pop ebp
	ret
