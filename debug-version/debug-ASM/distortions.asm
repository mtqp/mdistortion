;aqui se declaran extern las funciones que se usen
extern hall_asm

%define buf_out [ebp+8]
%define md_ptr  [ebp+12]
%define nframes [ebp+16]

section .data
	hs_cte : dd 1000.0
	_vol_anterior : dd 1

section .text
	global hell_sqr
	global by_pass

%include "m_macros.asm"

log_rock:			;log rock distortion function!
	convencion_C
	
	necesito_calcular_vol md_ptr, _vol_anterior, 3, no_lrock_calcvol
	
	calcular_volumen _vol_anterior, hs_cte, 3 			
;	void log_rock(float* out, m_distortion *mdc, int nframes){
;	int i = 0;
;	float vol = mdc->_vctes->log_rock_v+(mdc->_vctes->log_rock_v*mdc->_dvol);
;	for(i;i<nframes;i++){
;		out[i] = equalizer_effect(mdc,out[i],i);
;		out[i] = delay_effect(mdc,out[i],i);
;		out[i] = hall_effect(mdc,out[i],i);
;		out[i]=vol*sin(cos(log(sin(log(out[i])))));
;	}
no_lrock_calcvol:
	mov esi,nframes
	mov ebx,md_ptr
	mov edi,buf_out

ciclo_lrock:
	movdqu	xmm0,[edi]	;xmm0 = first 4 smps
	;call eq
	;call delay
	;call hall
		
	
	
	convencion_C_fin

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
	
;-------------------------------
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
