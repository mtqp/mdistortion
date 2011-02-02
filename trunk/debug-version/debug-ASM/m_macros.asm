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



