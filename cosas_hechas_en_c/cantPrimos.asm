extern malloc
global cantPrimos ; falta
global primosDivisoresA ;falta
global agregarN
global damePos;falta!

%define nro32bits	[ebp+8] ;LA NECESITO?

section .text:

listaPrimos* primosdivisoresA (int a)
cant primos ofsdvjdfoijvd
void agregarN(int dato, listaprimos*lp)
int damePos(int subi, listap* lp)

agregarN:
	%define puntLP	[ebp+8]
	%define dato 	[ebp+12]
	tam_nodo equ 8    ;CUAL ES EL TAM DEL NODO? 8 NO?
	push ebp
	mov ebp,esp
	push esi
	push edi
	push ebx 	;creo stack frame y convencion c
		
	mov ecx,puntLP	;puntero al puntero al 1er nodo
	mov edx,dato	;guardo el dato
	push tam_nodo
	call malloc
	add esp,tam_nodo

	push puntLP	;VEO SI TENGO Q AGREGAR PRIM O ULT 
	call listaVacia
	add esp,4
	cmp eax,0x0
	jz agregarPrimerNodo
	jmp ciclo	;agregar atras!!!

	
	;codigo copiado filminas
	malloc
	add esp,4
	mov [eax+offset_siguiente],0
	mov [eax+offset_dato],edx

	mov edx, [ecx]
	lea edx, [edx+offset_siguiente]

ciclo:
	cmp edx, offset_siguiente
	jz fin
	mov ecx, edx
	mov edx, [edx]
	lea edx, [edx+offset_siguiente]
	jmp ciclo
fin:
	mov [ecx], eax
	ret	;termina codigo copiado filminas


	;ES DIFERENTE PONER EL PRIMER NODO A UNO ULTIMO?
agregarPrimerNodo:
	push 8
	call malloc
	pop edx,8			;hacer el malloqueo


	pseudocod:
tengo q conseguir el puntero al primer nodo, si es null creo ahi el dato
si no es null, busco si el siguiente es null o me corro al siguieente
cuando encuentro uno apuntando a null (tengo q tener el anterior)
agarro y lo pongo, y hago apuntar ese q tenia null al NUEVO,
 y el nuevo entra en null
acordarse q hay q malloquear
tengo q reconfigurar los punteros fin y prim


	pop ebx		;restauro stack frame y convencion c
	pop edi
	pop esi
	add esp,8
	pop ebp
	ret		;termino

primosDivisoresA:
	push ebp
	mov ebp,esp
	;sub esp,8 VOY A GUARDAR ALGUNA VAR AUX?
	push esi
	push edi
	push ebx

	afsdfsd

	pop ebx
	pop edi
	pop esi
	;add esp,8 asdpiofasdijfsdf
	pop ebp
	ret
	


	
