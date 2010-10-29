
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
	f_effect[e_volume]		= &volume_func;
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
		md->_delay->dl_bufs[i] = NULL;
	}
	free(md->_delay->dl_bufs);
	md->_delay->dl_bufs = NULL;
	free(md->_delay);
	md->_delay = NULL;		///hasta aca el free de delay
	
	for(i=0;i<md->_hall->hll_buf_quantity;i++){
		free(md->_hall->hll_bufs);
		md->_hall->hll_bufs = NULL;
	}
	free(md->_hall->hll_bufs);
	md->_hall->hll_bufs = NULL;
	free(md->_hall);		
	md->_hall = NULL;		//hasta aca el free de hall
	
	free(md->m_bass);
	free(md->m_treb);
	free(md->m_mid);
	md->m_bass = NULL;
	md->m_treb = NULL;
	md->m_mid  = NULL;		//hasta aca el free de eq
	
	free(md->_vctes);		
	md->_vctes = NULL;		//hasta aca el free de volumen ctes
	
	free(md);				//m_distortion and effects freeded (no se si existe esa palabra!)

	printf("Freeing M_DISTORTION succesfull\n");
}

/*Llama al distosión* actual*/
void distortionize(m_distortion *md, jack_default_audio_sample_t *out, jack_nframes_t nframes){
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
void log_rock(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i = 0;
	float vol = mdc->_vctes->log_rock_v+(mdc->_vctes->log_rock_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		out[i]=vol*sin(cos(log(sin(log(out[i])))));
	}
}

void log_rock2(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){//
	int i = 0;
	float vol = mdc->_vctes->log_rock2_v+(mdc->_vctes->log_rock2_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);		
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		out[i]=  vol*cos(tan(tan(log((out[i])))));
	}
}


void psychedelic_if(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i = 0;
	float vol = mdc->_vctes->psyche_v+(mdc->_vctes->psyche_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);		
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		if(i < nframes/3) {
			out[i] = vol*(log(out[i])*10000.0)/5;
		} else {
			out[i] = vol*sin(log(sin(out[i])));
		}
	}
}

void by_60s(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i=0;
	float vol = mdc->_vctes->by_60s_v+(mdc->_vctes->by_60s_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		out[i] = vol*(100.0 * out[i]);
	}
}

void fuzzy_dark_pow4(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i = 0;
	float vol = mdc->_vctes->fuzzy_dark_v+(mdc->_vctes->fuzzy_dark_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);
		out[i] = vol*(100000000.0*(-pow(out[i],4)));
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
	}

}

void rare_cuadratic(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i = 0;
	float vol = mdc->_vctes->rare_c_v+(mdc->_vctes->rare_c_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		out[i] = vol*(11000.0*(pow(out[i],2)));
	}
}

void random_day(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	srand(time(NULL));
	int mod2;
	mod2 = rand()%7;
	if(mod2 == 0) 			log_rock(out,mdc,nframes);
	if(mod2 == 1)			log_rock2(out,mdc,nframes);
	if(mod2 == 2)			hell_sqr(out,mdc,nframes);
	if(mod2 == 3)			psychedelic_if(out,mdc,nframes);
	if(mod2 == 4)			by_60s(out,mdc,nframes);
	if(mod2 == 5)			fuzzy_dark_pow4(out,mdc,nframes);
	if(mod2 == 6)			rare_cuadratic(out,mdc,nframes);
}	
/*
void by_pass(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i=0;
   	for (i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		out[i] = delay_effect(mdc,out[i],i);
   	}
}*/

void mute(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i=0;
	for(i;i<nframes;i++){
		out[i] = 0.0;
	}
}
