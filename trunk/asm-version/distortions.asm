;aqui se declaran extern las funciones que se usen

%include "m_macros.asm"
%include "effects.asm"

%define buf_out [ebp+8]
%define md_ptr  [ebp+12]
%define nframes [ebp+16]

section .data
	hs_cte : dd 1000.0
	hs_vol_anterior : dd 1

section .text
	global hell_sqr
	global by_pass

hell_sqr:

	convencion_C 

	necesito_calcular_vol md_ptr, hs_vol_anterior, 3, no_hs_calcvol
	
	calcular_volumen hs_vol_anterior, hs_cte, 3 
	
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

ciclo_bp:
	movdqu	xmm0,[edi]	;xmm0 = first 4 smps
	
	mov  eax,esi
	sub  eax,nframes
	push esi			;push i
	;push xmm0			;push [smp...smp+4]
	push ebx			;push m_dist
	call hall			;//esto ESTA LLAMANDO SIEMPRE A HALL EH!

	movdqu	[edi],xmm0	;[out[i]...out[i+4]] = xmm7[i%4];
	
	lea edi,[edi+16]	;out* += 4;
	
	sub esi,4			;n -= 4;
	cmp esi,0			;¿n==0?
	je fin_bp
	jmp ciclo_bp
	
fin_bp:
	convencion_C_fin
