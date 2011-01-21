;aqui se declaran extern las funciones que se usen

extern rand		;funcion de C q retorna un numero aleatorio
extern hall_asm

%define buf_out [ebp+8]
%define md_ptr  [ebp+12]
%define nframes [ebp+16]

section .data
	hs_cte : dd 1000.0
	_vol_anterior : dd 1
;-------------para funciones matematicas -------------
	;anda barbaro definirlas asi y subirlas... para asi evitar hacer el shuffle... es un poco mas 
	;de mem por CREO YO mucha mas velocidad.
	menosUno:	dd -1.0,-1.0,-1.0,-1.0
	unoFact:	dd 1.0,1.0,1.0,1.0
	dosFact: 	dd 2.0,2.0,2.0,2.0
	tresFact: 	dd 6.0,6.0,6.0,6.0
	cuatroFact:	dd 24.0,24.0,24.0,24.0
	cincoFact:	dd 120.0,120.0,120.0,120.0
	seisFact:	dd 720.0,720.0,720.0,720.0
	sieteFact:	dd 5040.0,5040.0,5040.0,5040.0
	ochoFact:	dd 40320.0,40320.0,40320.0,40320.0
	nueveFact:  dd 362880.0,362880.0,362880.0,362880.0
	tres:		dd 3.0,3.0,3.0,3.0
	cuatro:		dd 4.0,4.0,4.0,4.0
	cinco:		dd 5.0,5.0,5.0,5.0
	seis:		dd 6.0,6.0,6.0,6.0
	siete:		dd 7.0,7.0,7.0,7.0
	ocho:		dd 8.0,8.0,8.0,8.0
	nueve:		dd 9.0,9.0,9.0,9.0
	fdp4_cte:	dd 100000000.0,100000000.0,100000000.0,100000000.0
	rarecd_cte: dd 11000.0,11000.0,11000.0,11000.0
	by60s_cte:	dd 100.0,100.0,100.0,100.0
	psyif1a_cte:dd 10000.0,10000.0,10000.0,10000.0
	psyif1b_cte:dd 5.0,5.0,5.0,5.0
	
;-----------------------------------------------------

section .text
	global hell_sqr
	global by_pass
	global by_60s
	global log_rock
	global log_rock2
	global fuzzy_dark_pow4
	global rare_cuadratic
	global mute
	global random_day
	global psychedelic_if

%include "m_macros.asm"
%include "maths_functions.asm"

;|||||||||||||||||||||||LOG-ROCK||||||||||||||||||||||||||||
log_rock:			;log rock distortion function!
	convencion_C

;	necesito_calcular_vol md_ptr, _vol_anterior, 3, no_lrock_calcvol
;	calcular_volumen _vol_anterior, hs_cte, 3 			

no_lrock_calcvol:
	mov esi,nframes
	mov ebx,md_ptr
	mov edi,buf_out

ciclo_lrock:
	movdqu	xmm0,[edi]	;xmm0 = first 4 smps
	;call eq
	;call delay
	;call hall

	asmLog
	asmSin
	asmLog
	asmCos
	asmSin
	;multiply volume	

	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm0[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub esi,4			;n -= 4;
	cmp esi,0			;¿n==0?
	jne ciclo_lrock

fin_lrock:	
	convencion_C_fin
;|||||||||||||||||||||||0000000000||||||||||||||||||||||||||||

;|||||||||||||||||||||||LOG-ROCK-II|||||||||||||||||||||||||||
log_rock2:			;log rock II distortion function!
	convencion_C

;	necesito_calcular_vol md_ptr, _vol_anterior, 3, no_lrock_calcvol
;	calcular_volumen _vol_anterior, hs_cte, 3 			

no_lrock2_calcvol:
	mov esi,nframes
	mov ebx,md_ptr
	mov edi,buf_out

ciclo_lrock2:
	movdqu	xmm0,[edi]	;xmm0 = first 4 smps
	;call eq
	;call delay
	;call hall

	asmLog
	asmTan
	asmTan
	asmCos
	;multiply volume	

	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm0[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub esi,4			;n -= 4;
	cmp esi,0			;¿n==0?
	jne ciclo_lrock2

fin_lrock2:	
	convencion_C_fin
;|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

;||||||||||||||||||||FUZZYDARKPOW4||||||||||||||||||||||||||||
fuzzy_dark_pow4:			;fuzzy dark pow 4 distortion function!
	convencion_C

;	necesito_calcular_vol md_ptr, _vol_anterior, 3, no_lrock_calcvol
;	calcular_volumen _vol_anterior, hs_cte, 3 			

no_fdp4_calcvol:
	mov esi,nframes
	mov ebx,md_ptr
	mov edi,buf_out

ciclo_fdp4:
	movdqu	xmm0,[edi]	;xmm0 = first 4 smps

	;call eq

	mov		eax,fdp4_cte
	mov 	ebx,menosUno
	movdqu	xmm1,[eax]
	movdqu	xmm2,[ebx]
	
	mulps	xmm0,xmm0	;XMM0 = BUF^2
	mulps	xmm0,xmm0	;XMM0 = BUF^4
	mulps	xmm0,xmm2	;xmm0 = -BUF^4
	mulps	xmm0,xmm1	;xmm0 = (100000000.0*(-pow(out[i],4)))
	
	;multiply volume	
	;call delay
	;call hall

	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm0[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub esi,4			;n -= 4;
	cmp esi,0			;¿n==0?
	jne ciclo_fdp4	

fin_fdp4:	
	convencion_C_fin

;|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

;!!!!!!!!!!!!!!!!!!!!!!!HELL-SQRT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
hell_sqr:			;hell square distortion function!
	convencion_C 

	necesito_calcular_vol md_ptr, _vol_anterior, 3, no_hs_calcvol
	
	calcular_volumen _vol_anterior, hs_cte, 3 			
	
no_hs_calcvol:
	mov esi,nframes ;contador ciclo
	mov ebx,md_ptr
	mov edi,buf_out	;buf_salida

ciclo_hs:
	movdqu	xmm0,[edi]	;xmm0 = first 4 smps
	sqrtps	xmm0,xmm0	;xmm0 = [sqrt(out1),sqrt(out2),sqrt(out3),sqrt(out4)]
	mulps 	xmm0,xmm7	;xmm0 = first 4 res;
	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm0[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub esi,4			;n -= 4;
	cmp esi,0			;¿n==0?
	je fin_hs
	jmp ciclo_hs
	
fin_hs:
	convencion_C_fin
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

;|||||||||||||||||||RARE-CUADRATIC||||||||||||||||||||||||||
rare_cuadratic:			;rare cuadratic distortion function!
	convencion_C

;	necesito_calcular_vol md_ptr, _vol_anterior, 3, no_lrock_calcvol
;	calcular_volumen _vol_anterior, hs_cte, 3 			

no_rc_calcvol:
	mov esi,nframes
	mov ebx,md_ptr
	mov edi,buf_out

ciclo_rc:
	movdqu	xmm0,[edi]	;xmm0 = first 4 smps

	;call eq
	;call delay
	;call hall

	mov		eax,rarecd_cte
	movdqu	xmm1,[eax]

	mulps	xmm0,xmm0	;XMM0 = BUF^2
	mulps	xmm0,xmm1	;xmm0 = (11000.0*(pow(out[i],2)));
	;multiply volume	

	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm0[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub esi,4			;n -= 4;
	cmp esi,0			;¿n==0?
	jne ciclo_rc	

fin_rc:	
	convencion_C_fin
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

;|||||||||||||||||||||||||BY-PASS||||||||||||||||||||||||||
by_pass:
	convencion_C

	mov esi,nframes ;contador ciclo
	mov ebx,md_ptr
	mov edi,buf_out	;buf_salida
	xor eax,eax

ciclo_bp:
	movdqu	xmm0,[edi]	;xmm0 = first 4 smps
	
	push 	eax			;push i (offset)
	push  	ebx			;push m_dist
	;call 	hall_asm	;//esto ESTA LLAMANDO SIEMPRE A HALL EH!
	;call 	dummy_asm
	add 	esp,8		;recupero el sp de los push q hice

	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm0[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	add eax,4			;i += 4;
	cmp eax,nframes		;¿i==nframes?
	jne ciclo_bp
	
fin_bp:
	convencion_C_fin
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

;||||||||||||||||||||||||MUTE||||||||||||||||||||||||||||||
mute:
	convencion_C

	mov esi,nframes ;contador ciclo
	mov edi,buf_out	;buf_salida

ciclo_mute:
	pxor 	xmm0,xmm0
	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm0[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	add eax,4			;i += 4;
	cmp eax,nframes		;¿i==nframes?
	jne ciclo_mute
	
fin_mute:
	convencion_C_fin
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

;|||||||||||||||||||||BY60S||||||||||||||||||||||||||||||||
by_60s:			;by_60s distortion function!
	convencion_C

;	necesito_calcular_vol md_ptr, _vol_anterior, 3, no_lrock_calcvol
;	calcular_volumen _vol_anterior, hs_cte, 3 			

no_by60s_calcvol:
	mov esi,nframes
	mov ebx,md_ptr
	mov edi,buf_out

ciclo_by60s:
	movdqu	xmm0,[edi]	;xmm0 = first 4 smps

	;call eq
	;call delay
	;call hall

	mov		eax,by60s_cte
	movdqu	xmm1,[eax]

	mulps	xmm0,xmm1	;xmm0 = 100.0*out[i];
	;multiply volume	

	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm0[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub esi,4			;n -= 4;
	cmp esi,0			;¿n==0?
	jne ciclo_by60s	

fin_by60s:	
	convencion_C_fin
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

;|||||||||||||||||||RANDOM-DAY|||||||||||||||||||||||||||||
random_day:			;random day distortion function!
	convencion_C

	call 	rand	;eax = random_number
					;busqueda binaria hardcodeada a 7 posibilidades (una por cada distor)
					
	mov 	ecx,eax	;ecx = bak(random_number)
	mov 	ebx,7
	xor		edx,edx
	div 	ebx		;edx = resto(rand/7)
		
	push dword nframes
	push dword md_ptr
	push dword buf_out

	cmp 	dword edx,3
	je 		picall
	jl		minor3
mayor3:
	cmp 	dword edx,5
	je		fdp4		;fuzzy dark pow 4
	jl  	b6s			;by 60s
	call 	rare_cuadratic
	jmp		fin_random_day

minor3:
	cmp 	dword edx,1
	je		lr2			;log roc ii
	jl		lr1			;log rock
	call 	hell_sqr
	jmp 	fin_random_day	
	
fdp4:
	call 	fuzzy_dark_pow4
	jmp 	fin_random_day

b6s:
	call 	by_60s
	jmp 	fin_random_day	
	
lr2:
	call 	log_rock2
	jmp 	fin_random_day

lr1:
	call 	log_rock
	jmp 	fin_random_day

picall:
	call 	psychedelic_if
	jmp 	fin_random_day
	
fin_random_day:	
	add 	esp,12 	;restauro esp
	
	convencion_C_fin
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
;void psychedelic_if(float* out, m_distortion *mdc, int nframes){
;	int i = 0;
;	float vol = mdc->_vctes->psyche_v+(mdc->_vctes->psyche_v*mdc->_dvol);
;	for(i;i<nframes;i++){
;		out[i] = equalizer_effect(mdc,out[i],i);		
;		out[i] = delay_effect(mdc,out[i],i);
;		out[i] = hall_effect(mdc,out[i],i);
;		if(i < nframes/3) {
;			out[i] = vol*(log(out[i])*10000.0)/5;
;			printf("-%d-",i);
;		} else {
;			out[i] = vol*sin(log(sin(out[i])));
psychedelic_if:			;psychedelic_if distortion function!
	convencion_C

;	necesito_calcular_vol md_ptr, _vol_anterior, 3, no_lrock_calcvol
;	calcular_volumen _vol_anterior, hs_cte, 3 			

no_psyif_calcvol:
	mov esi,nframes
	mov ebx,md_ptr
	mov edi,buf_out

	mov eax,esi
	mov ecx,3
	xor edx,edx
	div ecx			;eax = nframes / 3
	
;	sub	esi,ecx		;esi = nframes - nframes/3
	
ciclo_psyif1:
	movdqu	xmm0,[edi]	;xmm0 = first 4 smps

	;call eq
	;call delay
	;call hall

	push eax		;ESTOY CORTO DE REGISTROS ARREGLAR GOD DAMNINT!
	asmLog
	pop	 eax

	mov 	ecx,psyif1a_cte
	movdqu 	xmm1,[ecx]
	mov 	ecx,psyif1b_cte
	movdqu 	xmm2,[ecx]
	
	mulps 	xmm0,xmm1
	divps 	xmm0,xmm2

	;multiply volume	
	
	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm0[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub	esi,4			;nframes -= 4
	sub eax,4			;n -= 4;
	cmp eax,4			;¿n>4?
	jg  ciclo_psyif1	

psyif_reg_medio:
	movdqu xmm0,[edi]
	movdqu xmm7,xmm0		;xmm7 = back(buf)
	;libres xmm5,xmm6,xmm7
	;call eq
	;call delay
	;call hall
	asmSin
	asmLog
	asmSin
	;multiply volume		;xmm0 = [b1,b2,b3,b4]

	movdqu	xmm6,xmm0
	movdqu	xmm0,xmm7
	movdqu	xmm7,xmm6		;swap(xmm0,xmm7)
	
	asmLog
	mov 	ecx,psyif1a_cte
	movdqu 	xmm1,[ecx]
	mov 	ecx,psyif1b_cte
	movdqu 	xmm2,[ecx]
	mulps 	xmm0,xmm1
	divps 	xmm0,xmm2
	;multiply volume
	
							;xmm0 = 1era distor
							;xmm7 = 2da distor				
;---------------------------------------------------	
	pxor 	xmm5,xmm5
	cmpps	xmm5,xmm5,0	;xmm5 = [nan,nan,nan,nan]
;	movdqu	xmm6,xmm5	;xmm6 = [nan,nan,nan,nan]
	
	cmp edx,1
	jg	mayorduno
	jl	cont_reg_medio
shuno:						;1er float distor1, floats 2 3 y 4 distor 2
	psrldq 	xmm5,4
	pslldq 	xmm5,4			;xmm5 = [0,nan,nan,nan]
	pand	xmm7,xmm5		;xmm7 = [0,buf,buf,buf]
	
	psrldq  xmm5,12			;xmm5 = [nan,0,0,0]
	pand	xmm0,xmm5		;xmm0 = [buf,0,0,0]
	
	por		xmm0,xmm7		;xmm0 = [psy1(buf),psy2(buf),psy2(buf),psy2(buf)]
	
	jmp cont_reg_medio
mayorduno:
	cmp edx,3
	je	shtres
	jl	shdos
shcuatro:					;--> era puramente distorsion tipo 2
	movdqu	xmm7,xmm0	
	jmp cont_reg_medio
shtres:						;floats 1 2 3 distor1, float 4 distor 2
	pslldq	xmm5,4
	psrldq	xmm5,4			;xmm5 = [nan,nan,nan,0]
	pand	xmm0,xmm5		;xmm0 = [psy1(buf),psy1(buf),psy1(buf),0]

	psrldq	xmm5,12
	pand	xmm7,xmm5		;xmm7 = [0,0,0,psy2(buf)]

	por 	xmm0,xmm7
	jmp cont_reg_medio
		
shdos: 	
	pslldq	xmm5,8
	psrldq	xmm5,8			;xmm5 = [nan,nan,0,0]
	pand	xmm0,xmm5		;xmm0 = [psy1(buf),psy1(buf),0,0]

	pslldq	xmm5,8			;xmm5 = [0,0,nan,nan]
	pand	xmm7,xmm5		;xmm7 = [0,0,psy2(buf),psy2(buf)]
	
	por		xmm0,xmm7		;xmm0 = [psy1(buf),psy1(buf),psy2(buf),psy2(buf)]
;---------------------------------------------------	
cont_reg_medio:
	movdqu 	[edi],xmm0
	
	lea	edi,[edi+16]		;posiciono a siguiente buffer
	sub esi,4
ciclo_psyif2:
	movdqu	xmm0,[edi]	;xmm0 = first 4 smps

	;call eq
	;call delay
	;call hall

	asmSin
	asmLog
	asmSin

	;multiply volume	
	
	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm0[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub esi,4			;n -= 4;
	cmp esi,0			;¿n==0?
	jne ciclo_psyif2

fin_psyif:	
	convencion_C_fin

;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
