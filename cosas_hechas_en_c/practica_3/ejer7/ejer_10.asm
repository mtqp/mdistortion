;section .data
	
;extern void erosion(unsigned char *A, unsigned char *B, int m, int n);
section .text
	global erosion
	
erosion:

	%define ptr_A 	[ebp+8]
	%define ptr_B 	[ebp+12]
	%define m		[ebp+16]
	%define n		[ebp+20]
	
	;xmm0 = [p0,p1,p2,p3,p4,...,p15]
	;xmm1 = [i0,i1,i2,i3,i4,...,i15]
	;xmm2 = [x0,x1,x2,x3,x4,...,x15]
	;res_0 = min (p0,p1,p2,i0,i1,i2,x0,x1,x2) = min (min(p0,i0,x0),min(p1,i1,x1),min(p2,i2,x2))
	;res_1 = min (p1,p2,p3,i1,i2,i3,x1,x2,x3) = min (min(p3,i3,x3),min(p1,i1,x1),min(p2,i2,x2))
	;res_2 = min (p2,p3,p4,i2,i3,i4,x2,x3,x4) = min (min(p4,i4,x4),min(p3,i3,x3),min(p2,i2,x2))

	push ebp
	mov ebp,esp
	push esi
	push edi
	push ebx
	
	mov eax,ptr_A			;paso las variables a registros
	mov ebx,ptr_B
	mov ecx,n
	mov edx,m
	xor esi,esi				;contador filas
	xor edi,edi				;boolean (estoy en la ultima pasada de linea???)
	add ebx,ecx
	inc ebx					;ebx = ptr(primerLugarDondeGuardar)

ciclo_erosion:
	movdqu xmm0,[eax]		;xmm0 = [p0,p1,p2,p3,p4,...,p15]
	movdqu xmm1,[eax+ecx]	;xmm1 = [i0,i1,i2,i3,i4,...,i15]
	movdqu xmm2,[eax+2*ecx]	;xmm2 = [x0,x1,x2,x3,x4,...,x15]
	
	pminub xmm0,xmm1
	pminub xmm0,xmm2		;xmm0 = [min(i0,p0,x0),min(p1,i1,x1),min(p2,i2,x2)...]
	
	movdqu xmm2,xmm0
	movdqu xmm1,xmm0		;xmm0 == xmm1 == xmm2
	
	psrldq xmm1,1			;xmm1 = [min(p1,i1,x1),min(p2,i2,x2),min(p3,i3,x3),...,0]
	psrldq xmm2,2			;xmm2 = [min(p2,i2,x2),min(p3,i3,x3),min(p4,i4,x4),.,0,0]
	
	pminub xmm0,xmm1
	pminub xmm0,xmm2		;xmm0 = [B1,B2,B3,...,B13,FRUTA14,FRUTA15]

;////////////

	movdqu [ebx],xmm0		;movTOres
	
	cmp edi,0
	jne nueva_linea
	
	add ebx,14
	
	add eax,14				;apunto a nueva info a levantar

	add esi,14				;if (puedo seguir) then sigo else

	cmp esi,ecx				;if (mePASE) then hago la ultima linea de vuelta
	jl puedo_seguir
	jmp ult_linea
	
puedo_seguir:				;puedo_seguir =>laburar c/la nueva tira NO se pasa de la linea
	add esi,14
	cmp esi,ecx
	jge ult_linea
	sub esi,14
	jmp ciclo_erosion

ult_linea:
	;mov edi,ecx
	;sub edi,16
	;mov esi,edi				;off
	dec eax
	dec eax
	dec ebx 
	dec ebx					;¿ebx+14? NOOOOO ebx+12... xq nos qdan los ultimos 16 x procesar

	mov edi,1				;estoy procesando la ultima linea ahora
	
nueva_linea
	add eax,16				;eax = *ptr a linea nueva
	add ebx,16				;eax = *ptrDST a linea nueva
	mov esi,0				;esi = contadorLinea = 0
	mov edi,0				;edi = boolean ultimo pros linea 0
	dec ebx					;ebx = cantFilasXProcesar -1;
	cmp ebx,1
	jne nueva_linea
	
fin_erosion:
	pop ebx
	pop edi
	pop esi
	pop ebp
	ret
