
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
void log_rock(float* out, m_distortion *mdc, int nframes){
	int i = 0;
	float vol = mdc->_vctes->log_rock_v+(mdc->_vctes->log_rock_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		out[i] = vol*(taylorSin(taylorCos(taylorLog(taylorSin(taylorLog(out[i]))))));
	}
}

void log_rock2(float* out, m_distortion *mdc, int nframes){//
	int i = 0;
	float vol = mdc->_vctes->log_rock2_v+(mdc->_vctes->log_rock2_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);		
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		out[i] = vol*taylorCos(taylorTan(taylorTan(taylorLog(out[i]))));
	}
}


void hell_sqr(float* out, m_distortion *mdc, int nframes){
	int i = 0;
	float vol = mdc->_vctes->hell_sqr_v+(mdc->_vctes->hell_sqr_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i]= vol*(1000.0*sqrt(out[i]));
	}
}


void psychedelic_if(float* out, m_distortion *mdc, int nframes){
	int i = 0;
	float vol = mdc->_vctes->psyche_v+(mdc->_vctes->psyche_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);		
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		if(i < nframes/3) {
			out[i] = vol*(taylorLog(out[i])*10000.0)/5;
		} else {
			out[i] = vol*(taylorSin(taylorLog(taylorSin(out[i]))));
		}
	}
}

void by_60s(float* out, m_distortion *mdc, int nframes){
	int i=0;
	float vol = mdc->_vctes->by_60s_v+(mdc->_vctes->by_60s_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		out[i] = vol*(100.0 * out[i]);
	}
}

void fuzzy_dark_pow4(float* out, m_distortion *mdc, int nframes){
	int i = 0;
	float vol = mdc->_vctes->fuzzy_dark_v+(mdc->_vctes->fuzzy_dark_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);
		out[i] = vol*(100000000.0*(-pow(out[i],4)));
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
	}

}

void rare_cuadratic(float* out, m_distortion *mdc, int nframes){
	int i = 0;
	float vol = mdc->_vctes->rare_c_v+(mdc->_vctes->rare_c_v*mdc->_dvol);
	for(i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);
		out[i] = delay_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		out[i] = vol*(11000.0*(pow(out[i],2)));
	}
}

void random_day(float* out, m_distortion *mdc, int nframes){
	srand(time(NULL));
	unsigned int mod2;
	mod2 = rand()%7;
	if(mod2 == 0) 			log_rock(out,mdc,nframes);
	if(mod2 == 1)			log_rock2(out,mdc,nframes);
	if(mod2 == 2)			hell_sqr(out,mdc,nframes);
	if(mod2 == 3)			psychedelic_if(out,mdc,nframes);
	if(mod2 == 4)			by_60s(out,mdc,nframes);
	if(mod2 == 5)			fuzzy_dark_pow4(out,mdc,nframes);
	if(mod2 == 6)			rare_cuadratic(out,mdc,nframes);
}	

void by_pass(float* out, m_distortion *mdc, int nframes){
	int i=0;
   	for (i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		out[i] = delay_effect(mdc,out[i],i);
   	}
}

void mute(float* out, m_distortion *mdc, int nframes){
	int i=0;
	for(i;i<nframes;i++){
		out[i] = 0.0;
	}
}

