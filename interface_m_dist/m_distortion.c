#include "m_distortion.h"

#include <jack/jack.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void init_m_distortion(m_distortion * md){
	printf("inicializando m_distortion\n");
	float gain = 1.0;
	float var = 1.0;
//	md->_d_ch = (m_distortion_channel *) malloc(sizeof(m_distortion_channel));
	
	md->_dvol = 0.0;	//esta si la usamos
	md->_dgain= gain;	//esto esta al pedo
	md->_variacion_vol  = var;	//al pedo
	md->_variacion_gain = var;	//al re pedo

	//////parte de eq//////
	md->m_eq = (EQSTATE*) malloc(sizeof(EQSTATE));
	set_3band_state(md->m_eq,880,5000,480000);
	
	global_ptr = (globals*) malloc(sizeof(globals));
	
	global_ptr->dt = 0.1;
	global_ptr->RC = 1.0;
	global_ptr->alpha = global_ptr->RC / (global_ptr->RC+global_ptr->dt);
	global_ptr->plot_x = 0;

	global_ptr->_noise_toggled = 0;
	//////////////////////

	md->_d_active = 2;			
	md->_last_dist_active = md->_d_active;
	md->_cant_distors 	= 10;
	
	md->_name_dists[0] = "log_rock";
	md->_name_dists[1] = "log_rockII";
	md->_name_dists[2] = "hell sqr";
	md->_name_dists[3] = "psychedelic_if";
	md->_name_dists[4] = "by_60s";
	md->_name_dists[5] = "fuzzy_dark_pow4";
	md->_name_dists[6] = "rare_cuadratic";
	md->_name_dists[7] = "random_day";
	md->_name_dists[8] = "mute";
	md->_name_dists[9] = "by_pass";
	
	f_dist[0] = &log_rock;
	f_dist[1] = &log_rock2;
	f_dist[2] = &hell_sqr;
	f_dist[3] = &psychedelic_if;
	f_dist[4] = &by_60s;
	f_dist[5] = &fuzzy_dark_pow4;
	f_dist[6] = &rare_cuadratic;
	f_dist[7] = &random_day;	
	f_dist[8] = &mute;
	f_dist[9] = &by_pass;

	distortion_channel  = f_dist[md->_d_active]; 
	printf("\n\nm_distortion inicializada\n\n");
}

void free_m_distortion(m_distortion *md){
	free (md);
	free (global_ptr);
	printf("freeing m_distortion exitoso\n");
}

void distortionize(m_distortion *md, jack_default_audio_sample_t *out, jack_nframes_t nframes){
	distortion_channel(out, md, nframes);
}

void set_m_distortion( m_distortion * md, int dist){//name_dists *dist/*, unsigned char right*/){
	md->_d_active = dist;
	distortion_channel = f_dist[md->_d_active];
}

float actual_gain (m_distortion *mdc){
	return mdc->_dgain;
}

void gain_up(m_distortion *mdc){
	mdc->_dgain -= mdc-> _variacion_gain;	///if no alcanzo el max, ver como corregir eso
}


void gain_down(m_distortion *mdc){		//notar q no hay problema, el metodo distor se encarga de elevar⁴ este parametro tanto como en volumen
	mdc->_dgain += mdc ->_variacion_gain;
}

/////////////////////////////////////////////////////
///----------------DISTORSIONES-------------------///
/////////////////////////////////////////////////////
///LAS CTES SE ENCARGAN DE ESTABILIZAR TODOS EN MISMO VOLUMENES, Y CON DISTORS COPADAS

void log_rock(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){//logaritmic rock //seno hace mas agudo// cos mas grave
	int i = 0;
	float vol; 
	if(mdc->_dvol<=0.95)vol = 0.25+(0.25*mdc->_dvol);
	else 				vol = 1.0;
	if(global_ptr->_eq_sensitive){
		printf("no estamos ecualizando todavia\n");
		for(i;i<nframes;i++){
			out[i]=vol*(sin(cos(log(sin(log(out[i])))))); /// ENTRA CON EL VOL MAX Q TIENE
		}
	} else {
		for(i;i<nframes;i++){
			out[i]=vol*sin(cos(log(sin(log(out[i]))))); /// ENTRA CON EL VOL MAX Q TIENE
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
		printf("no estamos ecualizando todavia\n");
		for(i;i<nframes;i++){
			out[i]= vol*cos(tan(tan(log((out[i])))));//todavia se puede poner un poquito ams fuerte pero nada mas
			//tan lo hace mas grave sintbm, pero menos q tan ///-sen mas grave todavia
		}
	} else {
		for(i;i<nframes;i++){
			out[i]= vol*cos(tan(tan(log((out[i])))));//todavia se puede poner un poquito ams fuerte pero nada mas
			//tan lo hace mas grave sintbm, pero menos q tan ///-sen mas grave todavia
		}
	}
}

void hell_sqr(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){//raiz nasty
//	printf("hell sqrt\n");
	int i = 0;
	float vol;
	if(mdc->_dvol<=0.95)vol = 0.015+(0.015*mdc->_dvol);
	else 				vol = 1.0;
	if(global_ptr->_eq_sensitive){
		printf("no estamos ecualizando todavia\n"); 
		for(i;i<nframes;i++){
			out[i]= vol*(1000.0*sqrt(out[i]));
		}
	} else {
		for(i;i<nframes;i++){
			out[i]= vol*(1000.0*sqrt(out[i]));
		}
	}
}

void psychedelic_if(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){ //arco tangente
//	printf("psychedelic if\n");
	int i = 0;
	if(global_ptr->_eq_sensitive){
		printf("no estamos ecualizando todavia\n");
//no funciona esa forma de volumen aca
		for(i;i<nframes;i++){
			if(i < nframes/3) {
				out[i] = /*vol*/((log(out[i])*10000.0)/5);
			} else {
				out[i] = /*vol*/(sin(log(sin(out[i]))));
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
	int i = 0;
	if(global_ptr->_eq_sensitive){
		printf("no estamos ecualizando todavia\n");
		//multiplicar x una constante le baja el volumen y la ganancia.
		for(i;i<nframes;i++){
			out[i]= (100.0 * out[i]); ///funciona de volumen-ganancia, fijarse q se puede hacer
		}
	} else {
		for(i;i<nframes;i++){
			out[i]= (100.0 * out[i]); ///funciona de volumen-ganancia, fijarse q se puede hacer
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
		printf("no estamos ecualizando todavia\n");
		for(i;i<nframes;i++){
			out[i]= vol*(100000000.0*(-pow(out[i],4))); //mas bien mutea la guitarra
		}
	} else {
		for(i;i<nframes;i++){
			out[i]= vol*(100000000.0*(-pow(out[i],4))); //mas bien mutea la guitarra
		}
	}
}

void rare_cuadratic(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){ //cuadratica RARISIMA
//	printf("rare cuadratic\n");
	int i = 0;
	if(global_ptr->_eq_sensitive){
		printf("no estamos ecualizando todavia\n");
		for(i;i<nframes;i++){
			out[i]= /*vol**/(11000.0*(pow(out[i],2)));
		}
	} else {
		for(i;i<nframes;i++){
			out[i]= (11000.0*(pow(out[i],2)));
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

void mute(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
//	printf("mute\n");
	int i=0;
	FILE *f_out;
	f_out = fopen("by_pass.dat","a+");
	for(i;i<nframes;i++){
		//out[i] = 0.0;
		fprintf(f_out,"%d %f\n",global_ptr->plot_by_pass,out[i]);
		global_ptr->plot_by_pass++;
	}
	fclose(f_out);
}

void by_pass(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes){
//	printf("by pass\n");
	FILE * f_out;
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
}

/*
void vol_up_md (m_distortion *md, speaker sp){
	if (md->_master_ch == master_on){		//esto quizas es redundante ya q master es un bool
		volume_up(md->_d_ch);

		printf("setting volume for both channels\n");
	} else {
		if(sp){
			volume_up(md->__d_ch);
		} else {
			volume_up(md->_s_right);
		}
	}
}

void vol_down_md (m_distortion *md, speaker sp){
	if (md->_master_ch){		//x ser  redundante ya q master es un bool no agrego comparacion
		volume_down(md->_s_left);
		volume_down(md->_s_right);
		printf("setting volume for both channels\n");
	} else {
		if(sp){
			volume_down(md->_s_left);
		} else {
			volume_down(md->_s_right);
		}
	}
}

void gain_up_md (m_distortion *md, speaker sp){
	if (md->_master_ch){
		gain_up(md->_s_left);
		gain_up(md->_s_right);
		printf("setting gain for both channels\n");
	} else {
		if(sp){
			gain_up(md->_s_left);
		} else {
			gain_up(md->_s_right);
		}
	}
}

void gain_down_md (m_distortion *md, speaker sp){
	if (md->_master_ch){
		gain_down(md->_s_left);
		gain_down(md->_s_right);
		printf("setting gain for both channels\n");
	} else {
		if(sp){
			gain_down(md->_s_left);
		} else {
			gain_down(md->_s_right);
		}
	}
}

void reset_gain(m_distortion *md, speaker sp){
	if (md->_master_ch){
		md->_s_left->_dgain = 1.0;
		md->_s_right->_dgain = 1.0;
	} else {
		if (sp == speaker_izq){
			md->_s_left->_dgain = 1.0;
		} else {
			md->_s_right->_dgain = 1.0;		
		}
	}
}

void reset_vol(m_distortion *md, speaker sp){	
	if (md->_master_ch){
		md->_s_left->_dvol = 1.0;
		md->_s_right->_dvol = 1.0;
	} else {
		if (sp == speaker_izq){
			md->_s_left->_dvol = 1.0;
		} else {
			md->_s_right->_dvol = 1.0;		
		}
	}
}*/



/*
void change_master (m_distortion *md){
	if(md->_master_ch){
		printf("setting to master off\n");
		md->_master_ch = master_off;
	} else {
		printf("setting to master on\n");
		md->_master_ch = master_on;
		set_m_distortion(md,md->_d_left,0);			//el cero esta de mas, xq va a setear la distor izquierda
	}

}
*/
