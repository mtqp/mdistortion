
#include "m_effects.h"

/*
	Inicializa las estructura de los efectos
*/
void init_m_effects(m_distortion* md){
	printf("Init M_EFFECTS\n");
/////DELAY/////
	md->_delay = delay_new(262144,8);
/////HALL//////
	md->_hall  = hall_new(4096,4);
//////EQ//////
	eq_new(md);
	printf("M_EFFECTS initialized\n\n");
}

/*
	Inicializa la estructura ecualizador.
	Hay tres estructuras, una para cada banda.
*/
void eq_new(m_distortion* md){
	md->m_bass = band_EQ_new(LPF, 15.0, 800.0, 4096.0,6.5);
	md->m_treb = band_EQ_new(HPF, 4.0, 2000.0, 880096.0, 5.5);
	md->m_mid  = band_EQ_new(BPF, 15.0, 5000.0, 20000.0, 10.5); 
	if((md->m_bass==NULL) || (md->m_treb==NULL) || (md->m_mid == NULL)) 
		printf("Couldn't Set EQ\n");
	else
		printf("	EQ Set\n");
}

/*
	Función que ecualiza el sample pasado como parámetro.
*/
float equalizer_func(m_distortion *md, float smp, int i){
	smp = equalize_sample(smp, md->m_bass);
	smp = equalize_sample(smp, md->m_treb);
	return equalize_sample(smp, md->m_mid);
}

/*
	Función que aplica efecto delay sobre el sample de parámetro.
*/
float delay_func(m_distortion *md, float smp, int i){
	float old_smp;
	int bufs_active = md->_delay->dl_cant_bufs_active;
	int j;
	for(j=0;j<bufs_active;j++){
		old_smp = smp;
		smp += md->_delay->dl_bufs[j][md->_delay->dl_sub_i];
		md->_delay->dl_bufs[j][md->_delay->dl_sub_i] = old_smp;
	}
	
	md->_delay->dl_sub_i++;
	if(md->_delay->dl_sub_i >= md->_delay->dl_speed){	
		md->_delay->dl_sub_i = 0;
	}
	
	return smp;
}

/*
	Función que aplica efecto hall sobre el sample de parámetro.
*/
float hall_func(m_distortion *md, float smp, int i){
	int j;
	float save_smp;
	for(j=0;j<md->_hall->hll_buf_quantity;j++){
		save_smp = smp;
		smp += md->_hall->hll_bufs[j][i];
		md->_hall->hll_bufs[j][i] = md->_hall->hll_coef*(save_smp/2);
	}
	return smp;
}

/*
	Función dummy que no modifica el sample, necesaria cuando
	NO se quiere aplicar un efecto a la señal
*/
float dummy_func(m_distortion *md, float smp, int i){
	return smp;
}
