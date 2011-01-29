
#include "m_effects.h"

/*
	Inicializa las estructura de los efectos
*/
void init_m_effects(m_distortion* md){
	printf("Init M_EFFECTS\n");
	
/////DELAY/////
	md->_delay = delay_new(262144,8);

/////HALL//////
	md->_hall  = hall_new(1024,4);
	printf(":::::::::::::::::::::::::::\n");
	printf("posicion de mem de hall %d\n", (int) md->_hall);
	printf("buf quantity! %d\n", (int) md->_hall->hll_buf_quantity);
	printf("pos mem buf quantity! %d\n", (int) &(md->_hall->hll_buf_quantity));

//////EQ//////
	eq_new(md);

	printf("bass* =%d\n",(int)md->m_bass);
	printf("mid* =%d\n",(int)md->m_mid);	
	printf("treb* =%d\n",(int)md->m_treb);
	
	printf("bass ao=%f\n",md->m_bass->a0[0]);
	printf("mid ao=%f\n",md->m_mid->a0[0]);
	printf("treb ao=%f\n",md->m_treb->a0[0]);

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
	Función que aplica efecto delay sobre el sample de parámetro.

float delay_func(m_distortion *md, float smp, int i){
	float old_smp;
	int bufs_active = md->_delay->dl_cant_bufs_active;
	int j;
	for(j=0;j<bufs_active;j++){
		old_smp = smp;
		smp += md->_delay->dl_bufs[j][md->_delay->dl_sub_i];
		md->_delay->dl_bufs[j][md->_delay->dl_sub_i] = old_smp;
	}
	
	//esto va como funcion aparte!//////////////////////////
	///////////////////////////////////////////////////////POR AHORA NO LO PUSE FIJARSE
	/// SI ES ESTRICTAMENTE NECESARIO1!!!!!!
	for(j=bufs_active;j<md->_delay->dl_total_bufs;j++){
		md->_delay->dl_bufs[j][md->_delay->dl_sub_i] = 0.0;	
	}
	//////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	//CUANDO SE RESETEE LA CANTIDAD DE DISTORS ANDANDO SE CLEANEE
	//parche provisorio mandar cero (cambiar de 4 a 1 y de vuelta a 4)
	//podria llegar a hacerse un memset... hay q ver si puedo mantener el real time y mejora la eficiencia
	//para limpiar cuanod se cambia de velocidad tbm andaria un memset, pero no se si se justifica
	
	md->_delay->dl_sub_i++;
	
	if(md->_delay->dl_sub_i >= md->_delay->dl_speed){	
		md->_delay->dl_sub_i = 0;
	}
	
	return smp;
}*/


