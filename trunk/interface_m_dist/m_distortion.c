
#include "m_distortion.h"

void init_m_distortion(m_distortion * md){
	printf("\nInit M_DISTORTION\n");

	/////VOLUMEN////
	md->_dvol = 0.0;
	vol_new(md->_vctes);

	/////DELAY/////
	md->_delay = delay_new(262144,8);		//really big buffer for long delays q no baje hasta mucho mas q 65536
	//md->_delay = delay_new(65536);	//siempre es multiplo de 4096, xq ese es el buffer de jack, no olvidarse!

	/////HALL//////
	md->_hall = hall_new(4096,4);	 	//four small buffers for hall

	//////EQ//////
	eq_new(md);
	
	///////GLOBALS///////
	/*global_ptr = (globals*) malloc(sizeof(globals));
	global_ptr->_eq_sensitive   = sensitivo;
	global_ptr->plot_x = 0;		//es muy posible q no se necesite mas*/

	/////DISTORTIONS////
	md->_last_dist_active = e_hell_sqrt;

	f_dist[e_log_rock] 			= &log_rock;
	f_dist[e_log_rock_II] 		= &log_rock2;
	f_dist[e_hell_sqrt] 		= &hell_sqr;
	f_dist[e_psychedelic_if]	= &psychedelic_if;
	f_dist[e_by_60s] 			= &by_60s;
	f_dist[e_fuzzy_dark_pow_IV] = &fuzzy_dark_pow4;
	f_dist[e_rare_cuadratic] 	= &rare_cuadratic;
	f_dist[e_random_day] 		= &random_day;	
	f_dist[e_mute] 				= &mute;
	f_dist[e_by_pass] 			= &by_pass;
	printf("	Distortions Effects Set\n");

	f_effect[e_equalizer]	= &equalizer_func;
	f_effect[e_delay]		= &delay_func;
	f_effect[e_hall]		= &hall_func;
	f_effect[e_volume]		= &volume_func;
	f_effect[e_dummy]		= &dummy_func;

	equalizer_effect = f_effect[e_dummy];
	hall_effect 	 = f_effect[e_dummy];
	delay_effect  	 = f_effect[e_dummy];

	distortion_channel  = f_dist[md->_last_dist_active]; 
	printf("M_DISTORTION initialized\n\n");
}

void free_m_distortion(m_distortion *md){
	free(md->_vctes);
	free(md->m_bass);
	free(md->m_mid);
	free(md->m_treb);
	free (md);
	//free (global_ptr);
	//falta freerear el delay ahora...
	printf("Freeing M_DISTORTION succesfull\n");
}

void eq_new(m_distortion* md){
	md->m_bass = band_EQ_new(LPF, 15.0, 800.0, 4096.0,6.5);
	md->m_treb = band_EQ_new(HPF, 4.0, 2000.0, 880096.0, 5.5);
	md->m_mid  = band_EQ_new(BPF, 15.0, 5000.0, 20000.0, 10.5); 
	if((md->m_bass==NULL) || (md->m_treb==NULL) || (md->m_mid == NULL)) 
		printf("Couldn't Set EQ\n");
	else
		printf("	EQ Set\n");
}

void distortionize(m_distortion *md, jack_default_audio_sample_t *out, jack_nframes_t nframes){
	distortion_channel(out, md, nframes);
}

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

void log_rock(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i = 0;
	float vol; 
	if(mdc->_dvol<=0.95)vol = 0.25+(0.25*mdc->_dvol);
	else 				vol = 1.0;
	/*if(global_ptr->_eq_sensitive){
//		printf("En C no nos alcanza el REAL-TIME para ecualizar\n");
		for(i;i<nframes;i++){
			out[i]=vol*(sin(cos(log(sin(log(out[i]))))));
		}
	} else {
		for(i;i<nframes;i++){
			out[i] = delay_effect(mdc,out[i],i);
			out[i] = hall_effect(mdc,out[i],i);
			out[i]=vol*sin(cos(log(sin(log(out[i])))));
		}
	}*/
}

void log_rock2(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){//
//	printf("log rockII\n");
	int i = 0;
	float vol; 
	if(mdc->_dvol<=0.95)vol = 0.5+(0.5*mdc->_dvol);
	else 				vol = 1.0;
	/*if(global_ptr->_eq_sensitive){
//		printf("En C no nos alcanza el REAL-TIME para ecualizar\n");
		for(i;i<nframes;i++){
			out[i]= vol*cos(tan(tan(log((out[i])))));
		}
	} else {
		for(i;i<nframes;i++){
			out[i] = delay_effect(mdc,out[i],i);
			out[i] = hall_effect(mdc,out[i],i);
			out[i]= vol*cos(tan(tan(log((out[i])))));
		}
	}*/
}


void hell_sqr(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){//raiz nasty
//	printf("hell sqrt\n");
	int i = 0;
	float vol;
	if(mdc->_dvol<=0.95)vol = mdc->_vctes->hell_sqr_v+(mdc->_vctes->hell_sqr_v*mdc->_dvol);
	else 				vol = 1.0;

	for(i;i<nframes;i++){
		out[i]= vol*(1000.0*sqrt(out[i]));
	}
}


void psychedelic_if(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
//	printf("psychedelic if\n");
	int i = 0;
	float vol; 
	vol = 1.0+mdc->_dvol;

	/*if(global_ptr->_eq_sensitive){
//		printf("En C no nos alcanza el REAL-TIME para ecualizar\n");
		for(i;i<nframes;i++){
			if(i < nframes/3) {
				out[i] = vol*((log(out[i])*10000.0)/5);
			} else {
				out[i] = vol*(sin(log(sin(out[i]))));
			}
		}
	} else {
		for(i;i<nframes;i++){
			if(i < nframes/3) {
				out[i] = delay_effect(mdc,out[i],i);
				out[i] = hall_effect(mdc,out[i],i);
				out[i] = (log(out[i])*10000.0)/5;
			} else {
				out[i] = delay_effect(mdc,out[i],i);
				out[i] = hall_effect(mdc,out[i],i);
				out[i] = sin(log(sin(out[i])));
			}
		}
	}*/
}

void by_60s(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
//	printf("by 60s\n");
	float vol; 
	int i=0;
	vol = 1.0+mdc->_dvol;

		for(i;i<nframes;i++){
			out[i]= vol*(100.0 * out[i]);
			out[i] = delay_effect(mdc,out[i],i);
			out[i] = hall_effect(mdc,out[i],i);
		}

}

void fuzzy_dark_pow4(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){ //fuzzy oscura // siempre negativa.
//	printf("fuzzy dark pow4\n");
	int i = 0;
	float vol;
	if(mdc->_dvol<=0.95)vol = 0.0125+(0.0125*mdc->_dvol);//funciona de ganancia tbm un poco estaria bueno q cuando el volumen este al palo
	else 				vol = 1.0;	   					//el valor de vol sea 1 y asi no influya

		for(i;i<nframes;i++){
			out[i]= vol*(100000000.0*(-pow(out[i],4)));
			out[i] = delay_effect(mdc,out[i],i);
			out[i] = hall_effect(mdc,out[i],i);
		}

}

void rare_cuadratic(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){ //cuadratica RARISIMA
//	printf("rare cuadratic\n");
	int i = 0;
	float vol = 1.0+mdc->_dvol;
		for(i;i<nframes;i++){
			out[i] = delay_effect(mdc,out[i],i);
			out[i] = hall_effect(mdc,out[i],i);
			out[i]= vol*(11000.0*(pow(out[i],2)));
		}

}

void random_day(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
//	printf("random day\n");
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

void by_pass(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i=0;
   	for (i;i<nframes;i++){
		out[i] = equalizer_effect(mdc,out[i],i);
		out[i] = hall_effect(mdc,out[i],i);
		out[i] = delay_effect(mdc,out[i],i);
   	}
}

void mute(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i=0;
	
	for(i;i<nframes;i++){
		out[i] = 0.0;
	}
}

/////////////////////////////////////////////////////
///------------------EFFECTOS---------------------///
/////////////////////////////////////////////////////

float equalizer_func(m_distortion *md, float smp, int i){
	smp = equalize_sample(smp, md->m_bass);
	smp = equalize_sample(smp, md->m_treb);
	return equalize_sample(smp, md->m_mid);
}

float delay_func(m_distortion *md, float smp, int i){
	float old_smp;
	int bufs_active = md->_delay->dl_cant_bufs_active;
	int j;
	for(j=0;j<bufs_active;j++){
		old_smp = smp;
		smp += md->_delay->dl_bufs[j][md->_delay->dl_sub_i];
		md->_delay->dl_bufs[j][md->_delay->dl_sub_i] = old_smp;
	}
	
	for(j=bufs_active;j<md->_delay->dl_total_bufs;j++){
		md->_delay->dl_bufs[j][md->_delay->dl_sub_i] = 0.0;	
	}
	
	//CUANDO CAMBIA de 4 a uno menor y vuelve mete basura... limpiando los arrays safas... hay q ver q otra forma hay
	//parche provisorio mandar cero
	
	md->_delay->dl_sub_i++;
	if(md->_delay->dl_sub_i >= md->_delay->dl_speed){	
		md->_delay->dl_sub_i = 0;
	}
	
	return smp;
}

float hall_func(m_distortion *md, float smp, int i){
	int j;
	float save_smp;
	for(j=0;j<md->_hall->hll_buf_quantity;j++){	//EL HALL COEF FIJARSE SI SE NECESITA UNO SOLO O NO!
		save_smp = smp;
		smp += md->_hall->hll_bufs[j][i];
		md->_hall->hll_bufs[j][i] = md->_hall->hll_coef1*save_smp;
	}
	//VA A ESTAR MULTIPLICADO X CTES EENTRE 0 Y UNO Q REGULARAN LA CANTIDAD DE EFECTO
	return smp;
}

float volume_func(m_distortion *md, float smp, int i){
	return 0.0;//no creo poder tenerla en una funcion sepada xq las ctes son diferentes... ahh claro sisisi si puedo, para eso tengo la struct ctes!
}

float dummy_func(m_distortion *md, float smp, int i){
	return smp;
}

