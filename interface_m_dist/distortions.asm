;aqui se declaran extern las funciones que se usen

section .data
	hs_cte : dd 1000.0
	hs_vanterior : dd 1
section .text
	global hell_sqr

;acordarse de poner el condicional if para ecualizar o no ecualizar	

hell_sqr:
	%define buf_out [ebp+8]
	%define md_ptr  [ebp+12]
	%define nframes [ebp+16]

;	---------> convencion_C 
	push ebp
	mov ebp,esp
	push edi
	push esi
	push ebx

	mov 	ebx,md_ptr	;estr
	mov eax,[hs_vanterior]
	cmp dword eax,[ebx+12]
	je no_hs_calcvol
	
	mov eax,[ebx+12]
	mov dword [hs_vanterior],eax

hs_calcvol:
	pxor    xmm7,xmm7
	pxor 	xmm6,xmm6

	movss   xmm7,[ebx+12]	;[0,0,0,vol]
	
	lea ebx,[ebx+16]		;ptr_vol_ctes
	mov ebx,[ebx]
	lea ebx,[ebx+8]			;apunta a hellsqr_v
	movss	xmm6,[ebx]		;[0,0,0,0.15]

	mulss 	xmm7,xmm6		;
	addss	xmm7,xmm6		;[0,0,0,resvol]
	movlhps xmm7,xmm7		;[0,resvol,0,resvol]
	movdqu  xmm6,xmm7		;[0,resvol,0,resvol]
	pslldq  xmm6,4			;[resvol,0,resvol,0]
	addps   xmm7,xmm6		;[resvol,resvol,resvol,resvol] (0.015+(0.015*mdc->_dvol))

;-----en xmm7 esta el valor del vol-------
;-----------------------------------------
	pxor 	xmm6,xmm6
	pxor	xmm5,xmm5
	movss 	xmm6,[hs_cte]
	movlhps xmm6,xmm6
	movdqu 	xmm5,xmm6
	pslldq 	xmm5,4
	addps 	xmm6,xmm5		;xmm6 = [1000.0,1000.0,1000.0,1000.0]

	mulps	xmm7,xmm6		;xmm7 = [1000.0xresvol,1000.0xresvol,1000.0xresvol,1000.0xresvol]

;en xmm7 esta el valor de la cte multiplicativa
;----------------------------------------------
no_hs_calcvol:
	mov ecx,nframes ;contador ciclo
	mov ebx,md_ptr
	mov eax,buf_out	;buf_salida

ciclo_hs:
	cmp ecx,0
	je fin_hs
	
	movdqu	xmm0,[eax]
	sqrtps	xmm0,xmm0
	
	;jmp hs_equalize
	
hs_equalized:	
	mulps 	xmm0,xmm7
	movdqu	[eax],xmm0
	
	lea ebx,[ebx+16]
	lea eax,[eax+16]
	
	sub ecx,4
	jmp ciclo_hs
	
fin_hs:
;	-------> fin convencion C
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret

hs_equalize:
	;xmm7 intocable, xmm0 tengo la tira levantada, la devuelvo en xmm0 ecualizada
	;eax, ebx, ecx intocable... en ebx tengo md_ptr
	mov edx, ebx
	mov edx,[edx+8]		;//edx = ptr_eq
	
	movdqu 	xmm1,xmm0	;xmm0 = l, xmm1 = m, xmm2 = h
	movdqu 	xmm2,xmm0

;Filter LOWPASS
	;baja a memoria xmmI levanta con fpu xmmI y F1, calcula filtro 
	;baja a memoria L y F1 y sube a xmm L

;Filter HIGHPASS
	;baja a memoria xmmJ levanta con fpu xmmJ y F2, calcula filtro 
	;levante SDM3 y lo deja ahi (copia 4 floats iguales)
	;baja a memoria H y F2 y sube a xmm H

;Filter MIDRANGE
	;guarda en xmmh el midrange (o viceversa)
	;recalcula el array history

;Scale, combine, store
;  xmmi *= es->lg; -->multiplica x un array de 4 valores iguales.
;  xmmj *= es->mg;
;  xmmh	*= es->hg;


	addps 	xmm0, xmm1
	addps 	xmm0, xmm2	;return (l+m+h)
	jmp hs_equalized

