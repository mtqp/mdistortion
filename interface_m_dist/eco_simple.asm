%define origen  [ebp + 8]
%define destino [ebp + 12]

global ecosimple

;void ecosimple(wav_file* origen, wav_file* destino)
ecosimple:
        push    ebp
        mov     ebp, esp
        push esi
        push edi
        push ebx
       
        mov     esi, origen
        mov     esi, [esi + 16]
        mov     ecx, [esi + 4]  ; ecx = cantidad de muestras
        mov     esi, [esi + 8]  ; esi = muestras fuente
       
        mov     edi, destino
        mov     edi, [edi + 16]
        mov     edi, [edi + 8]  ; edi = muestras destino
       
        sub     ecx, 32		; como k = 4 y delay = 8, las ultimas 32 
        			; muestras no las proceso.
        shr     ecx, 3		; ecx = contador de ciclos
        
        jecxz	.fin		; si tenia menos de 40 muestras
        			; no proceso.
       
	pxor    xmm7, xmm7	; ceros para el unpack.
	
	.ciclo:

	        movq    xmm0, [esi]
                movq    xmm1, [esi + 8]
                movq    xmm2, [esi + 16]
                movq    xmm3, [esi + 24]
                movq    xmm4, [esi + 32]
               
                punpcklbw xmm0, xmm7
                punpcklbw xmm1, xmm7
                punpcklbw xmm2, xmm7
                punpcklbw xmm3, xmm7
                punpcklbw xmm4, xmm7
               
                psrlw   xmm1, 1
                psrlw   xmm2, 2
                psrlw   xmm3, 3
                psrlw   xmm4, 4
               
                paddusw xmm0, xmm1
                paddusw xmm0, xmm2
                paddusw xmm0, xmm3
                paddusw xmm0, xmm4
               
                packuswb xmm0, xmm7
                movq    [edi], xmm0
               
                add     esi, 8
                add     edi, 8
               
                loop    .ciclo
               
	.fin:
        	pop ebx
        	pop edi
        	pop esi
                pop ebp
                ret