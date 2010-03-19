extern malloc
extern potenciaC

section .data
	ptr_acumFCALLS: DD 0
	ptr_fpu_state: DD 0
	f_calls :DQ 0
	res_pila:DD 0
	
section .text
	global potencia
	global bashkara
	global fib
	
;---------------unsigned fib (unsigned n);---------------

fib:
	push ebp
	mov ebp,esp
	;sub esp,12	;ebp-12
	push esi	;ebp-16
	push edi	;ebp-20
	push ebx	;ebp-24
	
	%define n [ebp+8]
	;%define res_pila [ebp-4]	
	;%define f_calls [ebp-8]

	mov eax,8
	push eax
	call malloc
	add esp,4
	mov ebx,ptr_acumFCALLS
	mov [ebx],eax
	
	finit
	fld1			;1
	fld1			;1,1
	fadd st0,st0	;2,1
	fldz			;0,2,1
	fadd st0,st1	;2,2,1
	fmul st0,st0	;4,2,1
	fadd st0,st2	;5,2,1
	fsqrt			;sqrt(5),2,1
	fldz			;0,sqrt,2,1
	fadd st0,st1	;sqrt,sqrt,2,1
	fadd st0,st3	;1+sqrt,sqrt,2,1
	fdiv st0,st2	;PHI,sqrt,2,1
	fxch st2
	faddp st0,st0	;sqrt,PHI,1
	
	fxch st1
	fxch st2		;1,sqrt,PHI
	fsub st0,st2	;1-PHI,sqrt,PHI
	fldz			;0,1-PHI,sqrt,PHI	(le cargo el cero el call????)
	
	mov eax,108		;malloqueo estado fpu para salvarla
	push eax
	call malloc
	add esp,4
	
	fxch st3
	mov edx,f_calls
	fst qword [edx]	;guardo a PHI para calcular el potC
	fxch st3
	
	mov edi,n
	
	mov ebx,ptr_fpu_state
	mov [ebx],eax	;salvo el ptr_fpu_state con el malloc hecho
	fsave [eax]		;teoricamente guarda la fpu
	
	push qword [edx]	;double (phi)
	push edi			;int n
	mov esi,ptr_acumFCALLS
	push dword [esi];donde voy a guardar res

	call potenciaC
	add esp,16		;
					; quiero phi^n,1-phi,sqrt,phi
	
	mov dword eax,ptr_fpu_state
	mov eax,[eax]
	frstor [eax]	;restauro la fpu 0,1-PHI,sqrt,PHI
	faddp st0,st0	;1-phi,sqrt,phi
	mov ecx,ptr_acumFCALLS
	mov ecx,[ecx]
	fld qword [ecx]	;phi^n,1-phi,sqrt,phi
	
	fldz			;0,phi^n,1-phi,sqrt,phi
	
	fxch st2
	mov esi,f_calls
	fst qword [esi]
	fxch st2

	fsave [eax]
		
	push qword [esi]	
	push edi
	mov edi,ptr_acumFCALLS
	push dword [edi]

	call potenciaC  ;calculo (1-phi)^n
	add esp,16		
	
	mov eax,ptr_fpu_state
	mov eax,[eax]
	frstor [eax]	;restauro fpu
	faddp st0,st0
	mov ecx,ptr_acumFCALLS
	mov ecx,[ecx]
	fld qword [ecx]	;(1-phi)^n,phi^n,1-phi,sqrt,phi
	
	fsubp st1,st0	;numerador,1-phi,sqrt,phi
	fdiv st0,st2	;fib,1-phi,sqrt,phi
	
	mov edi,res_pila
	fisttp dword [edi]
	
	mov eax,[edi]
	
fin_fib:
	pop ebx
	pop edi
	pop esi
	;add esp,12
	pop ebp
	ret


	
;extern int bashkara(double a, double b, double c, double * r1, double * r2);
bashkara:
	;convencion_C////////
	push ebp
	mov ebp,esp
	push esi	;ebp-4
	push edi	;ebp-8
	push ebx	;ebp-12
	;////////////////////
	%define a [ebp+8] 
	%define b [ebp+16]
	%define c [ebp+24]
	%define r1 [ebp+32]		;estos dos son ptros!
	%define r2 [ebp+36]		;ptros (4bytes)
	
	xor eax,eax			;retorno cero x enunciado si jmpea
	finit
	fldz
	fld qword a				;sto=a && st1=0	(es cuadratica??)
	fcomip st0,st1
	
	jz fin_bashkara		;no es cuadratica!
	
	fld1
	fadd st0,st0		;st0 = 2
	fld st0				;[2,2]
	fadd st0,st1		;[4,2]
	fld qword a				;[a,4,2]
	fmul st2,st0		;[a,4,2a]
	fmulp				;[4a,2a]
	fld qword c				;[c,4a,2a]
	fmulp				;[4ac,2a]
	fld qword b				;[b,4ac,2a]
	fld st0				;[b,b,4ac,2a]
	
	fxch st3			;2a,b,4ac,b
	fxch st2			;4ac,b,2a,b
	fxch st1
						;[b,4ac,2a,b]
	fmul st0,st0		;[b²,4ac,2a,b]
	
	fsubrp st1,st0		;[b²-4ac,2a,b]
	fldz				;[0,BLAH,2a,b]
	fcomip st0,st1
	jc continuar		;!(=> blah>0)
	
esCeroEntonces:	
	jnz	fin_bashkara	;!(=> blah==0   => puedo continuar)
	
continuar:
	fsqrt				;[sqrt,2a,b]
	fld st0				;[sqrt,sqrt,2a,b]
	fxch st3			;b,sqrt,2a,sqrt
	fxch st2			;2a,sqrt,b,sqrt
	fxch st1			;sqrt,2a,b,sqrt
						;[sqrt,2a,b,sqrt]
	fadd st0,st2		;[b+sqrt,2a,b,sqrt]
	fxch st2			;[b,2a,b+sqrt,sqrt]
	fsub st0,st3		;b-sqrt,2a,b+sqrt,sqrt
	
	fdiv st0,st1		;r1,2a,b+sqrt,sqrt
	
	mov esi,r1
	fstp qword [esi]	;2a,b+sqrt,sqrt
	fdivp st1,st0		;r2,sqrt
	mov esi,r2
	fstp qword [esi]
	
	inc eax
fin_bashkara:
	;convencion_C_fin///
	pop ebx
	pop edi
	pop esi
	pop ebp
	
	ret


;///////////////////////
potencia:
	;//convencion_C//////
	push ebp
	mov ebp,esp
	push esi	;ebp-4
	push edi	;ebp-8
	push ebx	;ebp-12
	;////////////////////
	
	%define n [ebp+8]
	%define pot [ebp+16]
	
	finit 					;(inicio FPU)
	fld qword n			;en ST0 esta N
	fld qword n			;en ST0 esta el acum, y en st1 el multiplicador
	mov ebx,pot
ciclo_pot:
	fmul st0,st1			; ST0 = n², ST1 = n
	dec ebx
	cmp ebx,1
	jne ciclo_pot
	
fin_pot:					;devuelve siempre el parametro en ST0!!!! no olvidar!	
	;//convencion_C_fin//
	pop ebx
	pop edi
	pop esi
	pop ebp
	;////////////////////
	ret
