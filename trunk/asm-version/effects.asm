float hall_func(m_distortion *md, float smp, int i){
	int j;
	float save_smp;
	for(j=0;j<md->_hall->hll_buf_quantity;j++){
		save_smp = smp;
		smp += md->_hall->hll_bufs[j][i];
		md->_hall->hll_bufs[j][i] = md->_hall->hll_coef*(save_smp/2);
	}
	return smp;
} tengo q hacerlo para cuatro samples!

;ocupados ESI EBX EDI
;xmm0 y xmm7
hall:
	mov eax,esi
	sub eax,nframes		;eax = i
	
	mov ecx,md_ptr 
	lea ecx,[ecx+28]	;ecx = ptr_hall
	
	movdqu xmm1,xmm0		;save_smp = smp
	
	jmp after_hall
