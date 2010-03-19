;section .data
	
section .text
	global productoMatrizVector

;extern void productoMatrizVector(float* A, float* v,float* res, unsigned short n,unsigned short m);	
productoMatrizVector:

	%define A 		[ebp+8]
	%define v 		[ebp+12]
	%define res 	[ebp+16]
	%define short_n [ebp+20]
	%define short_m [ebp+24]

	push ebp
	mov ebp,esp
	push esi
	push edi
	push ebx
	
	mov eax,A
	mov ebx,v
	mov ecx,res
	pxor xmm7,xmm7
	
	xor esi,esi
	xor edi,edi
	mov si,short_n
	mov di,short_m

cicloCRAZY:
	movdqu xmm0,[eax]
	movdqu xmm1,[ebx]
	
	mulps xmm0,xmm1			;[a1*v1,a2*v2,a3*v3,a4*v4]
	
	addps xmm7,xmm0			;(res parcial)

	add eax,16				;apunto a nueva info a cargar
	add ebx,16
	
	sub esi,4				
	cmp esi,0
	jne cicloCRAZY			;termine la linea?
							;ahora si tengo q mandar el resultado a memoria
	movdqu xmm6,xmm7		;xmm7 ==[f0,f1,f2,f3] == xmm6
	psrldq xmm6,8			;xmm7==[f0,f1,f2,f3] && xmm6==[f2,f3,0,0]
	addps xmm7,xmm6			;xmm7==[f0+f2,f1+f3,f2,f3]
	movdqu xmm6,xmm7		;xmm6 = xmm7
	psrldq xmm6,4			;xmm6 = [f1+f3,f2,f3,0]
	addps xmm7,xmm6			;xmm7 == [RES,fruta,fruta,fruta]

	movss [ecx],xmm7		;mueve solo el 1er float a mem (res)

	mov si,short_n			;I_v = size(v);
	mov ebx,v				;*v = v.primero;
	add ecx,4				;I_res == I_res+1;	
	sub edi,1

	cmp edi,0				;termine?
	jne cicloCRAZY


fin_PMV:
	pop ebx
	pop edi
	pop esi
	pop ebp
	ret
