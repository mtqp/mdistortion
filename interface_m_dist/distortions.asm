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
	mov esi,nframes ;contador ciclo
	mov ebx,md_ptr
	mov edi,buf_out	;buf_salida

ciclo_hs:
	cmp esi,0
	je fin_hs
	
	movdqu	xmm0,[edi]
	
	;jmp fpu_hs_equalize


fpu_hs_equalized:	

;	sqrtps	xmm0,xmm0

	mulps 	xmm0,xmm7
	movdqu	[edi],xmm0
	
	lea ebx,[ebx+16]
	lea edi,[edi+16]
	
	sub esi,4
	jmp ciclo_hs
	
fin_hs:
;	-------> fin convencion C
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret

fpu_hs_equalize:
	mov edx, md_ptr
	mov edx,[edx+8]		;edx = ptr_eq
	
	movdqu [edx],xmm0		;eq->data = xmm0 (se levanta dsp desde fpu)
	
	finit
	;||sample 1||
	fld dword [edx+16]	;[lf]
	fld dword [edx+20]	;[p0,lf]
	fld dword [edx]		;cargo sample1 = s1 [s1,p0,lf]
	fsub st0,st1		;[s1-p0,p0,lf]
	fmul st0,st2		;[lf*(s1-p0),p0,lf]	
	faddp st1,st0		;[P0,lf]

	fld dword [edx+24]	;[p1,P0,lf]
	fldz				;[0,p1,P0,lf]
	fadd st0,st2		;[P0,p1,P0,lf]
	fsub st0,st1		;[P0-p1,p1,P0,lf]
	fmul st0,st3		;[lf*(P0-p1),p1,P0,lf]
	faddp st1,st0		;[P1,P0,lf]
	
	fld dword [edx+28]	;[p2,P1,P0,lf]
	fldz				;[0,p2,P1,P0,lf]
	fadd st0,st2		;[P1,p2,P1,P0,lf]
	fsub st0,st1		;[P1-p2,p2,P1,P0,lf]
	fmul st0,st4		;[lf*(P1,p2),p2,P1,P0,lf]
	faddp st1,st0		;[P2,P1,P0,lf]
	fst dword [edx]		;guardo primer sample
	
	;||sample2||
	fld dword [edx+4]	;[s2,p2,p1,p0,lf]
	fsub st0,st3		;[s2-p0,p2,p1,p0,lf]
	fmul st0,st4		;[lf*(s2-p0),p2,p1,p0,lf]
	faddp st3,st0		;[p2,p1,P0,lf]
	
	fldz				;[0,p2,p1,P0,lf]
	fadd st0,st3		;[P0,p2,p1,P0,lf]
	fsub st0,st2
	fmul st0,st4
	faddp st2,st0		;[p2,P1,P0,lf]
	
	fldz				;[0,p2,P1,P0,lf]
	fadd st0,st1		;[p2,p2,P1,P0,lf]
	fsub st0,st2
	fmul st0,st4
	faddp st1,st0		;[P2,P1,P0,lf]
	fst dword [edx+4]	;guardo segunda sample
	
	;||sample3||
	fld dword [edx+8]	;[s3,p2,p1,p0,lf]
	fsub st0,st3		;[s3-p0,p2,p1,p0,lf]
	fmul st0,st4		;[lf*(s3-p0),p2,p1,p0,lf]
	faddp st3,st0		;[p2,p1,P0,lf]
	
	fldz				;[0,p2,p1,P0,lf]
	fadd st0,st3		;[P0,p2,p1,P0,lf]
	fsub st0,st2
	fmul st0,st4
	faddp st2,st0		;[p2,P1,P0,lf]
	
	fldz				;[0,p2,P1,P0,lf]
	fadd st0,st1		;[p2,p2,P1,P0,lf]
	fsub st0,st2
	fmul st0,st4
	faddp st1,st0		;[P2,P1,P0,lf]
	fst dword [edx+8]	;guardo tercer sample
	
	;||sample4||
	fld dword [edx+12]	;[s4,p2,p1,p0,lf]
	fsub st0,st3		;[s4-p0,p2,p1,p0,lf]
	fmul st0,st4		;[lf*(s4-p0),p2,p1,p0,lf]
	faddp st3,st0		;[p2,p1,P0,lf]
	
	fldz				;[0,p2,p1,P0,lf]
	fadd st0,st3		;[P0,p2,p1,P0,lf]
	fsub st0,st2
	fmul st0,st4
	faddp st2,st0		;[p2,P1,P0,lf]
	
	fldz				;[0,p2,P1,P0,lf]
	fadd st0,st1		;[p2,p2,P1,P0,lf]
	fsub st0,st2
	fmul st0,st4
	faddp st1,st0		;[P2,P1,P0,lf]
	fst dword [edx+12]	;guardo cuarto sample

	fstp dword [edx+28]
	fstp dword [edx+24]
	fstp dword [edx+20]
	
	movdqu	xmm0,[edx]	;xmm0 = data
	movdqu 	xmm1,[edx+62] ;xmm1 = lg
	mulps	xmm0,xmm1	;l         *= es->lg;
	;como faltan los otros filtros no hacemos nada mas
	jmp fpu_hs_equalized

