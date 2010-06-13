
#include "m_distortion.h"

void init_m_distortion(m_distortion * md){
	printf("\nInit M_DISTORTION\n");

	/////VOLUMEN////
	md->_dvol = 0.0;
	vol_new(md->_vctes);

	/////DELAY/////
	md->_delay = delay_new(262144);	//really big buffer for long delays
	
	////CHORUS////
	md->_chorus = chorus_new(4096); //four small buffers for chorus

	//////EQ//////
	eq_new(md);
	
	///////GLOBALS///////
	global_ptr = (globals*) malloc(sizeof(globals));
	global_ptr->_chorus_toggled = not_def_toggled;
	global_ptr->_delay_toggled 	= not_def_toggled;
	global_ptr->_eq_sensitive   = sensitivo;
	global_ptr->plot_x = 0;		//es muy posible q no se necesite mas

	/////DISTORTIONS////
	md->_last_dist_active = e_hell_sqrt;

	md->_name_dists[e_log_rock] 		 = "log_rock";
	md->_name_dists[e_log_rock_II] 		 = "log_rockII";
	md->_name_dists[e_hell_sqrt] 		 = "hell sqr";
	md->_name_dists[e_psychedelic_if] 	 = "psychedelic_if";
	md->_name_dists[e_by_60s] 			 = "by_60s";
	md->_name_dists[e_fuzzy_dark_pow_IV] = "fuzzy_dark_pow4";
	md->_name_dists[e_rare_cuadratic] 	 = "rare_cuadratic";
	md->_name_dists[e_random_day] 		 = "random_day";
	md->_name_dists[e_mute] 			 = "mute";
	md->_name_dists[e_by_pass] 			 = "by_pass";
	md->_name_dists[e_delay]			 = "delay";
	
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
	f_dist[e_delay]				= &delay;
	printf("	Distortions Effects Set\n");

	distortion_channel  = f_dist[md->_last_dist_active]; 
	printf("M_DISTORTION initialized\n\n");
}

void free_m_distortion(m_distortion *md){
	free(md->_vctes);
	free(md->m_bass);
	free(md->m_mid);
	free(md->m_treb);
	free (md);
	free (global_ptr);
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
	if(global_ptr->_eq_sensitive){
		printf("En C no nos alcanza el REAL-TIME para ecualizar\n");
		for(i;i<nframes;i++){
			out[i]=vol*(sin(cos(log(sin(log(out[i]))))));
		}
	} else {
		for(i;i<nframes;i++){
			out[i]=vol*sin(cos(log(sin(log(out[i])))));
		}
	}
}

void log_rock2(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){//
//	printf("log rockII\n");
	int i = 0;
	float vol; 
	if(mdc->_dvol<=0.95)vol = 0.5+(0.5*mdc->_dvol);
	else 				vol = 1.0;
	if(global_ptr->_eq_sensitive){
		printf("En C no nos alcanza el REAL-TIME para ecualizar\n");
		for(i;i<nframes;i++){
			out[i]= vol*cos(tan(tan(log((out[i])))));
		}
	} else {
		for(i;i<nframes;i++){
			out[i]= vol*cos(tan(tan(log((out[i])))));
		}
	}
}


void hell_sqr(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){//raiz nasty
//	printf("hell sqrt\n");
	int i = 0;
	float vol;
	if(mdc->_dvol<=0.95)vol = mdc->_vctes->hell_sqr_v+(mdc->_vctes->hell_sqr_v*mdc->_dvol);
	else 				vol = 1.0;
	if(global_ptr->_eq_sensitive){
//		printf("no estamos ecualizando todavia\n"); 
		for(i;i<nframes;i++){
			out[i] = equalize_sample(out[i], mdc->m_bass);
			out[i] = equalize_sample(out[i], mdc->m_treb);
			out[i] = equalize_sample(out[i], mdc->m_mid);
			//out[i]= vol*(1000.0*sqrt(out[i]));
			//printf("outi == %f\n", out[i]);
		}
	} else {
		for(i;i<nframes;i++){
			out[i]= vol*(1000.0*sqrt(out[i]));
		}
	}
}

void psychedelic_if(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
//	printf("psychedelic if\n");
	int i = 0;
	float vol; 
	vol = 1.0+mdc->_dvol;

	if(global_ptr->_eq_sensitive){
		printf("En C no nos alcanza el REAL-TIME para ecualizar\n");
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
				out[i] = (log(out[i])*10000.0)/5;
			} else {
				out[i] = sin(log(sin(out[i])));
			}
		}
	}
}

void by_60s(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
//	printf("by 60s\n");
	float vol; 
	vol = 1.0+mdc->_dvol;

	int i = 0;
	if(global_ptr->_eq_sensitive){
		printf("En C no nos alcanza el REAL-TIME para ecualizar\n");
		for(i;i<nframes;i++){
			out[i]= vol*(100.0 * out[i]);
		}
	} else {
		for(i;i<nframes;i++){
			out[i]= vol*(100.0 * out[i]);
		}
	}
}

void fuzzy_dark_pow4(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){ //fuzzy oscura // siempre negativa.
//	printf("fuzzy dark pow4\n");
	int i = 0;
	float vol;
	if(mdc->_dvol<=0.95)vol = 0.0125+(0.0125*mdc->_dvol);//funciona de ganancia tbm un poco estaria bueno q cuando el volumen este al palo
	else 				vol = 1.0;	   					//el valor de vol sea 1 y asi no influya
	if(global_ptr->_eq_sensitive){
		printf("En C no nos alcanza el REAL-TIME para ecualizar\n");
		for(i;i<nframes;i++){
			out[i]= vol*(100000000.0*(-pow(out[i],4)));
		}
	} else {
		for(i;i<nframes;i++){
			out[i]= vol*(100000000.0*(-pow(out[i],4)));
		}
	}
}

void rare_cuadratic(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){ //cuadratica RARISIMA
//	printf("rare cuadratic\n");
	int i = 0;
	float vol = 1.0+mdc->_dvol;
	if(global_ptr->_eq_sensitive){
		printf("En C no nos alcanza el REAL-TIME para ecualizar\n");
		for(i;i<nframes;i++){
			out[i]= vol*(11000.0*(pow(out[i],2)));
		}
	} else {
		for(i;i<nframes;i++){
			out[i]= vol*(11000.0*(pow(out[i],2)));
		}
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

void delay(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i=0;
	float old_smp;
	float vol; 
	vol = 1.0+mdc->_dvol;
/*
	for(i;i<nframes;i++){
	//	out[i] *= vol*100.0;
		old_smp = out[i];
		out[i] += mdc->_delay->delay_buf[i];
		mdc->_delay->delay_buf[i] = old_smp;
		old_smp = out[i];
		out[i] += mdc->_delay->delay_buf_2_orden[i];
		mdc->_delay->delay_buf_2_orden[i] = old_smp;	
		old_smp = out[i];
		out[i] += mdc->_delay->big_delay[mdc->_delay->actual_big_delay];
		mdc->_delay->big_delay[mdc->_delay->actual_big_delay] = old_smp;
		
		mdc->_delay->actual_big_delay++;
		if(mdc->_delay->actual_big_delay == mdc->_delay->big_delay_size){	
			mdc->_delay->actual_big_delay = 0;
		}
	}*/
}

void mute(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
	int i=0;
	float old_smp;
/*	printf("delay process\n");
	printf("mdist pointer = %d\n", (int) mdc);
	printf("mdelay pointer = %d\n", (int) mdc->_delay);
	printf("delay_buf==%d\n",(int) mdc->_delay->delay_buf);
	for(i;i<nframes;i++){
		old_smp = out[i];
		out[i] += mdc->_delay->delay_buf[i];
		mdc->_delay->delay_buf[i] = old_smp;
		old_smp = out[i];
		out[i] += mdc->_delay->delay_buf_2_orden[i];
		mdc->_delay->delay_buf_2_orden[i] = old_smp;		
		old_smp = out[i];
		out[i] += mdc->_delay->delay_buf_3_orden[i];
		mdc->_delay->delay_buf_3_orden[i] = old_smp;		
		old_smp = out[i];
		out[i] += mdc->_delay->delay_buf_4_orden[i];
		mdc->_delay->delay_buf_4_orden[i] = old_smp;		
		out[i] /= 2;
	}*/
	/*
//	printf("mute\n");
	int i=0;
//	FILE *f_out;
//	f_out = fopen("by_pass.dat","a+");
	for(i;i<nframes;i++){
		out[i] = 0.0;
//		fprintf(f_out,"%d %f\n",global_ptr->plot_by_pass,out[i]);
//		global_ptr->plot_by_pass++;
	}
//	fclose(f_out);*/
}

void by_pass(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
//	printf("by pass\n");
/*	FILE * f_out;
	f_out = fopen("from_treb_to_bass.dat", "a+");
	
	if(global_ptr->_eq_sensitive){
	// Return RC high-pass filter output samples, given input samples,
	// time interval dt, and time constant RC
		float limpio_i;
		//float limpio_i_menos_uno;

		int i = 1;
		
		out[0] = out[0] + global_ptr->limpio_i_menos_uno;
	   
	   	for (i;i<nframes;i+=3){
	   		limpio_i 			= out[i];
	   		global_ptr->limpio_i_menos_uno 	= out[i-1];
	   		out[i+1] = 0.0;
	   		out[i+2] = 0.0;
	   		//out[i] = alpha * (out[i-1] + limpio_i - limpio_i_menos_uno);
	   		out[i] = (global_ptr->alpha*limpio_i) + (global_ptr->alpha*global_ptr->limpio_i_menos_uno);
	   		

			fprintf(f_out,"%d %f\n",global_ptr->plot_x,out[i]);
	   		global_ptr->plot_x +=3;
	   	}
	}
	fclose(f_out);
*/
}

