
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


