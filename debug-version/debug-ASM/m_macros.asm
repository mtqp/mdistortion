%macro convencion_C_mem 1
	push ebp
	mov ebp,esp
	sub esp,%1
	push edi
	push esi
	push ebx
%endmacro

%macro convencion_C_fin_mem 1
	pop ebx
	pop esi
	pop edi
	add esp,%1
	pop ebp
	ret
%endmacro

%macro convencion_C 0
	push ebp
	mov ebp,esp
	push edi
	push esi
	push ebx
%endmacro

%macro convencion_C_fin 0
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret
%endmacro

;md=val1, distor_vol_anterior=val2, posicion_distor_vol=val_3, label_no_calc_vol=val4
%macro necesito_calcular_vol 4
	mov 	  ebx,%1				;estr
	mov 	  eax,[%2]
	cmp dword eax,[ebx+(%3*4-4)]	;¿¿vol_anterior == md->_dvol??
	je  %4
%endmacro

;distor_vol_anterior == val_1 ;distor_cte == val_2 ;posicion_distor_vol == val_3
%macro calcular_volumen 3
	mov 	  eax,[ebx+4]			;eax = md->_dvol
	mov dword [%1],eax

hs_calcvol:
	pxor    xmm7,xmm7
	pxor 	xmm6,xmm6

	movss   xmm7,[ebx+4]	;[0,0,0,vol]
	
	lea ebx,[ebx+8]			;ptr_vol_ctes
	mov ebx,[ebx]
	lea ebx,[ebx+(%3*4-4)]	;apunta a (distor)_v
	movss	xmm6,[ebx]		;[0,0,0,distor_v]

	mulss 	xmm7,xmm6		;
	addss	xmm7,xmm6		;[0,0,0,resvol]
	movlhps xmm7,xmm7		;[0,resvol,0,resvol]
	movdqu  xmm6,xmm7		;[0,resvol,0,resvol]
	pslldq  xmm6,4			;[resvol,0,resvol,0]
	addps   xmm7,xmm6		;[resvol,resvol,resvol,resvol] (dist_v+(dist_v*mdc->_dvol)) == XMM7

;-----------------------------------------
	pxor 	xmm6,xmm6
	pxor	xmm5,xmm5
	movss 	xmm6,[%2]
	movlhps xmm6,xmm6		;ESTO SE PUEDE REEMPLAZAR POR UPLOAD CTE
	movdqu 	xmm5,xmm6
	pslldq 	xmm5,4
	addps 	xmm6,xmm5		;xmm6 = [d_cte,d_cte,d_cte,d_cte]

	mulps	xmm7,xmm6		;xmm7 = [cte*vol,cte*vol,cte*vol,cte*vol]
;----------------------------------------------
%endmacro

;carga en el 1er registro pasado como parametro la constante del tercer parametro. (utiliza un segundo registro xmmX provisorio)
%macro upload_cte 3		;parametros : [regDST,regIntermedio,floatASetear]	('regIntermedio' se modificara)
	pxor    %1,%1
	pxor    %2,%2
	movss   %1,[%3]		;%1=[0,0,0,cte]
	movlhps %1,%1		;%1=[0,cte,0,cte]
	movdqu  %2,%1		;%2=[0,cte,0,cte]
	pslldq  %2,4		;%2=[cte,0,cte,0]
	addps   %1,%2		;%1=[cte,cte,cte,cte]
%endmacro

;limpia de xmm0 tantos floats como numero haya en edx (esta entre 0 y 4)
;usa xmm5 de registro auxiliar
;utiliza busqueda binaria para hacer mas rapidos los calculos
;se tuvo q crear xq pslldq no acepta un registro, sino un imm8
;%macro mascara_y_limpiar
;	pxor 	xmm5,xmm5
;	cmpps	xmm5,xmm5,1	;xmm5 = [nan,nan,nan,nan]
	
;	cmp edx,2
;	je	shdos
;	jl	minordos
;mayordos:
;	cmp edx,3
;	je	shtres
;shcuatro:
;	pxor xmm5,xmm5
;	jmp 	shcero
;minordos:
;	cmp edx,0
;	je shcero
;shuno:
;	pslldq,4
;	pslrdq,4			;xmm5 = [0,nan,nan,nan]
;	jmp shcero
;shtres:
;	pslrdq,4
;	pslldq,4			;xmm5 = [nan,nan,nan,0]
;	jmp shcero	
;shdos: 	
;	pslldq,8
;	pslrdq,8
;shcero:
;%endmacro


;limpia de xmm0 tantos floats como numero haya en edx (esta entre 0 y 4)
;usa xmm5 de registro auxiliar
;utiliza busqueda binaria para hacer mas rapidos los calculos
;se tuvo q crear xq pslldq no acepta un registro, sino un imm8
;optimizado para buffer de 4096 y uso en psychedelic if!!!
%macro mascara_y_limpiar 1
	pxor 	xmm5,xmm5
	cmpps	xmm5,xmm5,1	;xmm5 = [nan,nan,nan,nan]
	
	cmp edx,1
	jg	mayoruno
	jl	%1
aaashuno:
	pslldq,4
	pslrdq,4			;xmm5 = [0,nan,nan,nan]
	jmp %1
aaamayorduno:
	cmp edx,3
	je	shtres
	jl	shdos
aaashcuatro:
	pxor xmm5,xmm5
	jmp  %1
aaashtres:
	pslrdq,4
	pslldq,4			;xmm5 = [nan,nan,nan,0]
	jmp %1	
aaashdos: 	
	pslldq,8
	pslrdq,8			;xmm5 = [0,0,nan,nan]
aaarightMask:
	pand	xmm0,xmm5
	jmp finMask
aaaleftMask:
	por		xmm0,xmm5	;xmm0 = [nro,nro,nan,nan]
	pxor	xmm0,xmm5	;xmm0 = [nro,nro,0,0]
aaafinMask:
%endmacro


