;aqui se declaran extern las funciones que se usen


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
;-----------------------------------------------------

section .text
	global hell_sqr
	global by_pass
	global log_rock
	global log_rock2

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

	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm7[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub esi,4			;n -= 4;
	cmp esi,0			;¿n==0?
	jne ciclo_lrock

fin_lrock:	
	convencion_C_fin
;|||||||||||||||||||||||0000000000||||||||||||||||||||||||||||

;|||||||||||||||||||||||LOG-ROCK-II|||||||||||||||||||||||||||
;void log_rock2(float* out, m_distortion *mdc, int nframes){//
;	int i = 0;
;	float vol = mdc->_vctes->log_rock2_v+(mdc->_vctes->log_rock2_v*mdc->_dvol);
;	for(i;i<nframes;i++){
;		out[i] = equalizer_effect(mdc,out[i],i);		
;		out[i] = delay_effect(mdc,out[i],i);
;		out[i] = hall_effect(mdc,out[i],i);
;		out[i]=  vol*cos(tan(tan(log((out[i])))));
;	}
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

	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm7[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub esi,4			;n -= 4;
	cmp esi,0			;¿n==0?
	jne ciclo_lrock2

fin_lrock2:	
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
	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm7[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub esi,4			;n -= 4;
	cmp esi,0			;¿n==0?
	je fin_hs
	jmp ciclo_hs
	
fin_hs:
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

	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm7[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	add eax,4			;i += 4;
	cmp eax,nframes		;¿i==nframes?
	jne ciclo_bp
	
fin_bp:
	convencion_C_fin
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
