;precondicion, de xmm0 se toma el buffer y en xmm0 se deja el buffer

%macro asmSin 0
	;UTILIZA: xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,eax
;	movdqu	xmm0,[esi]	
						;XMM0 = [buf[i],buf[i+1],buf[i+2],buf[i+3]] = POSITIVOS
						;XMM1 = NEGATIVOS
	movdqu  xmm3,xmm0	;XMM3 = [buf[i],buf[i+1],buf[i+2],buf[i+3]]	(NO se modifica)

	movdqu	xmm2,xmm3	;XMM2 = Provisorio POTENCIAS
	mulps	xmm2,xmm2	;XMM2 = BUF^2
	mulps	xmm2,xmm3	;XMM2 = BUF^3

	movdqu 	xmm1,xmm2	;XMM1 = pow(x[i,...,i+3],3)
	mov 	eax,tresFact
	movdqu  xmm4,[eax]	;XMM4 = ['x'Factorial,'x'Factorial,'x'Factorial,'x'Factorial]
	
	divps	xmm1,xmm4	;XMM1 = pow(x[i,...,i+3],3)/factorial(3)
;////////	
	mulps	xmm2,xmm3	;XMM2 = BUF^4
	mulps	xmm2,xmm3	;XMM2 = BUF^5

	movdqu  xmm5,xmm2   ;XMM5 = temporal para dividir por fact
	
	mov 	eax,cincoFact
	movdqu	xmm4,[eax]
	
	divps	xmm5,xmm4	;XMM5 = pow(x,5)/fact(5)
	addps	xmm0,xmm5	;XMM0 = [x+(x^5/5!),x+(x^5/5!),x+(x^5/5!),x+(x^5/5!)]
;////////	
	mulps	xmm2,xmm3	;XMM2 = BUF^6
	mulps	xmm2,xmm3	;XMM2 = BUF^7
	
	movdqu	xmm5,xmm2	
	
	mov 	eax,sieteFact
	movdqu  xmm4,[eax]
	
	divps	xmm5,xmm4	;XMM5 = X^7/7! [4]
	addps	xmm1,xmm5	;XMM1 = (x^3/3! + x^7/7!) [4]  = NEGATIVOS
;///////	
	mulps	xmm2,xmm3	;XMM2 = BUF^8
	mulps	xmm2,xmm3	;XMM2 = BUF^9
	
	movdqu	xmm5,xmm2	
		
	mov 	eax,nueveFact
	movdqu	xmm4,[eax]
	
	divps	xmm5,xmm4	;XMM5 = x^9/9! [4]
	addps	xmm0,xmm5	;XMM0 = (x + (x^5/5!) + (x^9/9!))[4] = POSITIVOS
;////////	
	subps	xmm0,xmm1   ;XMM0 = POSITIVOS - NEGATIVOS
;	movdqu  [esi],xmm0
%endmacro

%macro asmCos 0
	;UTILIZA: eax,xmm0,xmm1,xmm2,xmm3,xmm4,xmm5
	mov 	eax,unoFact
	movdqu	xmm1,[eax]		;XMM1 = 1.0[4]
	
;	movdqu	xmm0,[esi]		
							;XMM0 = buf[i,...,i+3]
	
	mulps	xmm0,xmm0		;XMM0 = buf^2[i,...,i+3]
	
	movdqu  xmm2,xmm0		;XMM2 = buf^2[4]
	mulps	xmm2,xmm0		;XMM2 = buf^4[4]
	
	movdqu	xmm3,xmm2		;XMM3 = buf^4[4]
	mulps	xmm3,xmm0		;XMM3 = buf^6[4]
	
	movdqu	xmm4,xmm2		;XMM4 = buf^4[4]
	mulps	xmm4,xmm4		;XMM4 = buf^8[4]
	
	mov 	eax,dosFact
	movdqu	xmm5,[eax]
	
	divps	xmm0,xmm5		;XMM0 = buf^2/2![4]
	
	mov 	eax,cuatroFact
	movdqu	xmm5,[eax]
	
	divps	xmm2,xmm5		;XMM2 = buf^4/4![4]
	
	mov 	eax,seisFact
	movdqu	xmm5,[eax]
	
	divps	xmm3,xmm5		;XMM3 = buf^6/6![4]
	
	mov 	eax,ochoFact
	movdqu	xmm5,[eax]
	
	divps	xmm4,xmm5		;XMM4 = buf^8/8![4]	
	
	subps	xmm1,xmm0
	addps	xmm1,xmm2
	subps	xmm1,xmm3
	addps	xmm1,xmm4
	
	movdqu	xmm0,xmm1
	
	;movdqu 	[esi],xmm0
%endmacro

%macro asmTan 0	
	;los mismos q sin y cos mas xmm7
;	movdqu	xmm0,[esi]
	movdqu	xmm7,xmm0		;XMM0 = buf[4]
	
;	push dword	i
;	push dword	buf
;	call 	asmCos
;	add 	esp,8

	asmCos	
;	movdqu 	xmm1,[esi]		;XMM1 = cos(buf)[4]
;	movdqu	[esi],xmm7		;RESTAURO en buf los valores originales
;	movdqu 	xmm7,xmm1		;xmm7 = cos(buf)[4]
	
	movdqu xmm1,xmm0
	movdqu xmm0,xmm7
	movdqu xmm7,xmm1		;swap xmm0,xmm7
	
;	push dword	i
;	push dword	buf
;	call 	asmSin
;	add 	esp,8

	asmSin	
;	movdqu	xmm0,[esi]		;XMM0 = sin(buf)[4]
	
	divps	xmm0,xmm7		;XMM1 = tan(buf)[4]
	;movdqu	[esi],xmm0
	;xmm7 = se utiliza para chequear si el valor debe ser nan o no (fijarse caso del cero)
%endmacro

%macro asmLog 0
	;xmm0 guarda el resultado
	;xmm1 guarda buf-1
	;xmm2 guarda la potencia anterior a la que se calcula
	;xmm3 calcula la nueva potencia y divide por la cte
	;xmm4 guarda la cte
	;eax  ayuda para levantar las ctes
	;movdqu	xmm0,[esi]	
						;XMM0 = buf[4]
	pxor	xmm1,xmm1
	cmpps	xmm1,xmm0,6	
	por		xmm0,xmm1	;xmm0 = todos los menores a cero, pasan a estar en valor nan

	mov 	eax,unoFact
	movdqu	xmm1,[eax]	;XMM1 = 1.0[4]
	
	subps	xmm0,xmm1	;XMM0 = (buf-1)[4]
	
	movdqu	xmm1,xmm0	;XMM1 = (buf-1)[4]
	movdqu	xmm2,xmm1	;XMM2 = (buf-1)[4]
	mulps	xmm2,xmm1	;XMM2 = (buf-1)^2[4]
	movdqu	xmm3,xmm2	;XMM3 = (buf-1)^2[4]
	
	mov		eax,dosFact
	movdqu	xmm4,[eax]	;XMM4 = 2.0[4]
	
	divps	xmm3,xmm4	;XMM3 = (buf-1)^2/2 [4]
	
	subps	xmm0,xmm3	;xmm0 = (buf-1)[4] - (buf-1)^2/2[4]
;////////	
	movdqu	xmm3,xmm2
	mulps	xmm3,xmm1	;xmm3 = (buf-1)^3[4]
	movdqu	xmm2,xmm3
	
	mov		eax,tres
	movdqu	xmm4,[eax]
	divps	xmm3,xmm4	;xmm3 = (buf-1)^3/3[4]
	
	addps	xmm0,xmm3	;xmm0 = (buf-1)[4] - (buf-1)^2/2[4] + (buf-1)^3/3[4]
;////////	
	movdqu 	xmm3,xmm2
	mulps	xmm3,xmm1	;xmm3 = (buf-1)^4[4]
	movdqu	xmm2,xmm3
		
	mov 	eax,cuatro
	movdqu	xmm4,[eax]
	divps	xmm3,xmm4	;xmm3 = (buf-1)^4/4[4]
	
	subps	xmm0,xmm3	;xmm0 = (buf-1)[4] - (buf-1)^2/2[4] + (buf-1)^3/3[4] - (buf-1)^4/4[4]
;////////	
	movdqu	xmm3,xmm2
	mulps	xmm3,xmm1	;xmm3 = (buf-1)^5[5]
	movdqu	xmm2,xmm3
	
	mov 	eax,cinco
	movdqu	xmm4,[eax]
	divps	xmm3,xmm4	;xmm3 = (buf-1)^5/5[5]
	
	addps	xmm0,xmm3	;xmm0 = (buf-1)[4] - (buf-1)^2/2[4] + (buf-1)^3/3[4] - (buf-1)^4/4[4] + (buf-1)^5/5[5]
;////////	
	movdqu	xmm3,xmm2
	mulps	xmm3,xmm1	;xmm3 = (buf-1)^6[4]
	movdqu	xmm2,xmm3
	
	mov		eax,seis
	movdqu	xmm4,[eax]
	divps	xmm3,xmm4	;xmm3 = (buf-1)^6/6[4]
	
	subps	xmm0,xmm3	;xmm0 = (buf-1)[4] - (buf-1)^2/2[4] + (buf-1)^3/3[4] - (buf-1)^4/4[4] + (buf-1)^5/5[4] 
;////////						;- (buf-1)^6/6[4]
	
	movdqu	xmm3,xmm2
	mulps	xmm3,xmm1	;xmm3 = (buf-1)^7[4]
	movdqu	xmm2,xmm3
	
	mov		eax,siete
	movdqu	xmm4,[eax]
	divps	xmm3,xmm4	;xmm3 = (buf-1)^7/7[4]
	
	addps	xmm0,xmm3	;xmm0 = (buf-1)[4] - (buf-1)^2/2[4] + (buf-1)^3/3[4] - (buf-1)^4/4[4] + (buf-1)^5/5[4] 
;////////						;- (buf-1)^6/6[4] + (buf-1)^7/7[4]
	
	movdqu	xmm3,xmm2
	mulps	xmm3,xmm1	;xmm3 = (buf-1)^8[4]
	movdqu	xmm2,xmm3
	
	mov		eax,ocho
	movdqu	xmm4,[eax]
	divps	xmm3,xmm4	;xmm3 = (buf-1)^8/8[4]
	
	subps	xmm0,xmm3	;xmm0 = (buf-1)[4] - (buf-1)^2/2[4] + (buf-1)^3/3[4] - (buf-1)^4/4[4] + (buf-1)^5/5[4] 
;////////					;- (buf-1)^6/6[4] + (buf-1)^7/7[4] - (buf-1)^8/8[4]

	movdqu	xmm3,xmm2
	mulps	xmm3,xmm1	;xmm3 = (buf-1)^9[4]
	movdqu	xmm2,xmm3
	
	mov		eax,nueve
	movdqu	xmm4,[eax]
	divps	xmm3,xmm4	;xmm3 = (buf-1)^9/9[4]
	
	addps	xmm0,xmm3	;xmm0 = (buf-1)[4] - (buf-1)^2/2[4] + (buf-1)^3/3[4] - (buf-1)^4/4[4] + (buf-1)^5/5[4] 
;////////						;- (buf-1)^6/6[4] + (buf-1)^7/7[4] - (buf-1)^8/8[4] + (buf-1)^9/9[4]
%endmacro
