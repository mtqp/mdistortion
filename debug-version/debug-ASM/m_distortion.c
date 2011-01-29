
#include "m_distortion.h"

/** Inicializa la estructura principal 
*y se encarga de llamar a la inicialización de
*las sub-estructuras
**/
void init_m_distortion(m_distortion * md){
	printf("\nInit M_DISTORTION\n");

	/////VOLUMEN////
	md->_dvol  = 0.0;
	md->_vctes = vol_new();

	/////DISTORTIONS////
	md->_last_dist_active = e_hell_sqrt;			//Setea la distorsión inicial

	f_dist[e_log_rock] 			= &log_rock;		//Almacena los function* a los distintos
	f_dist[e_log_rock_II] 		= &log_rock2;		//tipos de distorsiones
	f_dist[e_hell_sqrt] 		= &hell_sqr;
	f_dist[e_psychedelic_if]	= &psychedelic_if;
	f_dist[e_by_60s] 			= &by_60s;
	f_dist[e_fuzzy_dark_pow_IV] = &fuzzy_dark_pow4;
	f_dist[e_rare_cuadratic] 	= &rare_cuadratic;
	f_dist[e_random_day] 		= &random_day;	
	f_dist[e_mute] 				= &mute;
	f_dist[e_by_pass] 			= &by_pass;
	printf("	Distortions Effects Set\n");

	f_effect[e_equalizer]	= &equalizer_func;		//Almacena los function* a los distintos
	f_effect[e_delay]		= &delay_func;			//tipos de efectos
	f_effect[e_hall]		= &hall_func;
	f_effect[e_dummy]		= &dummy_func;

	equalizer_effect = f_effect[e_dummy];
	hall_effect 	 = f_effect[e_dummy];
	delay_effect  	 = f_effect[e_dummy];

	distortion_channel  = f_dist[md->_last_dist_active]; 
	printf("M_DISTORTION initialized\n\n");
	
}

void free_m_distortion_and_effects(m_distortion *md){
	/*Libera todas las estructuras del programa*/
	int i;
	for(i=0;i<md->_delay->dl_total_bufs;i++){
		free(md->_delay->dl_bufs[i]);
	}
	free(md->_delay->dl_bufs);
	free(md->_delay);
	
	for(i=0;i<md->_hall->hll_buf_quantity;i++){
		free(md->_hall->hll_bufs[i]);
	}
	free(md->_hall->hll_bufs);
	free(md->_hall);		
	
	free(md->m_bass);
	free(md->m_treb);
	free(md->m_mid);
	
	free(md->_vctes);		
	
	free(md);				//m_distortion and effects freeded (no se si existe esa palabra!)

	printf("Freeing M_DISTORTION succesfull\n");
}

/*Llama al distosión* actual*/
void distortionize(m_distortion *md, float* out, int nframes){
	distortion_channel(out, md, nframes);
}

/*Setea la distorsión pasada en parámetro 'dist' como la actual*/
void set_m_distortion( m_distortion * md, int dist){
	int cambia_modo = dist==e_random_day || dist==e_mute || dist==e_by_pass;
	if(dist==back_to_rock_mode)	
		dist = md->_last_dist_active;
	else 
		if(!cambia_modo)	
			md->_last_dist_active = dist;

	distortion_channel = f_dist[dist];
}

/////////////////////////////////////////////////////
///----------------DISTORSIONES-------------------///
/////////////////////////////////////////////////////

/*
  Cada una de las distorsiones se encarga de calcular el volumen
  aplicarle todos los efectos y distorsionar la señal,
  recibe por parámetro el buffer in, devolviendo el out en el
  mismo buffer
*/
