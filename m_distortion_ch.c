#include "m_distortion_ch.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string.h>
#include <jack/jack.h>

void init_m_distortion_ch(m_distortion_channel *d, float vol, float gain, float var_vol, float var_gain){
	printf("initialising m_distortion_channel\n");
	d->_dvol = vol;
	d->_dgain= gain;
	d->_pow_dvol = pow(vol,4);
	d->_pow_dgain= pow(gain,4);
	d->_variacion_vol  = var_vol;
	d->_variacion_gain = var_gain;

	//SETEAR TODAS LAS DISTORSIONES! EN EL ARRAY MALDITO INT! :d:d:d:d:
	
	f_dist[0] = &log_rock;
	f_dist[1] = &log_rock2;
	f_dist[2] = &hell_sqr;
	f_dist[3] = &psychedelic_if;
	f_dist[4] = &by_60s;
	f_dist[5] = &fuzzy_dark_pow4;
	f_dist[6] = &rare_cuadratic;
	f_dist[7] = &random_day;	
	
}

float actual_volume (m_distortion_channel *mdc){
	return mdc->_dvol;
}

void volume_up(m_distortion_channel *mdc){
	mdc->_dvol += mdc-> _variacion_vol;	///if no alcanzo el max, ver como corregir eso
	mdc->_pow_dvol = pow(mdc->_dvol,4);
}


void volume_down(m_distortion_channel *mdc){
	//if(mdc->_dvol > mdc->_variacion_vol){
		mdc->_dvol -= mdc ->_variacion_vol;
		mdc->_pow_dvol = pow(mdc->_dvol,4);
	//} else {
		//mdc->_dvol = 0.0;
	//}
}

float actual_gain (m_distortion_channel *mdc){
	return mdc->_dgain;
}

void gain_up(m_distortion_channel *mdc){
	mdc->_dgain += mdc-> _variacion_gain;	///if no alcanzo el max, ver como corregir eso
	mdc->_pow_dgain = pow(mdc->_dgain,4);
}


void gain_down(m_distortion_channel *mdc){		//notar q no hay problema, el metodo distor se encarga de elevar⁴ este parametro tanto como en volumen
	if(mdc->_dgain > mdc->_variacion_gain){
		mdc->_dgain -= mdc ->_variacion_gain;
		mdc->_pow_dgain = pow(mdc->_dgain,4);
	} else {
		mdc->_dgain = 0.0;
	}
}

/////////////////////////////////////////////////////
///----------------DISTORSIONES-------------------///
/////////////////////////////////////////////////////

///LAS CTES SE ENCARGAN DE ESTABILIZAR TODOS EN MISMO VOLUMENES, Y CON DISTORS COPADAS

void log_rock(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//logaritmic rock //seno hace mas agudo// cos mas grave
//	printf("log rock\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]=sin(cos(log(sin(log(out[i])))));//con seno tbm qda RE buena
	}
}

void log_rock2(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//
//	printf("log rock II\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= cos(tan(tan(log((out[i])))));//tan lo hace mas grave sintbm, pero menos q tan ///-sen mas grave todavia
	}
}

void hell_sqr(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//raiz nasty
//	printf("hell sqrt\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= 1000.0*sqrt(out[i]); ///se puede elevar hasta el cuadrado data y suena copado
	}
}

void psychedelic_if(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //arco tangente
//	printf("sweet_arc_tan\n");
	int i = 0;
	float vol = (mdc->_pow_dvol)/5;
	for(i;i<nframes;i++){
		if(i < nframes/3) {
			out[i] = (log(out[i])*10000.0)/5;
		} else {
			out[i] = sin(log(sin(out[i])));	 	//q la mejor aproximacion es la x4
		}
	}
}

void by_60s(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){
//	printf("by pass\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= 100.0 * out[i];
	}
}

void fuzzy_dark_pow4(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //fuzzy oscura // siempre negativa.
//	printf("fuzzy dark pow4\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= 100000000.0*(-pow(out[i],4));
	}
}

void rare_cuadratic(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //cuadratica RARISIMA
//	printf("rare_cuadratic\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= 11000.0*(pow(out[i],2));
	}
}

void random_day(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //sum(d/2^i) bastante normal
//	printf("raw sum 2i\n");
	srand(time(NULL));
	int mod2;
	/*switch (mod2 = rand()%7){
		case '0':
			log_rock(out,mdc,nframes);
			break;
		case '1':
			log_rock2(out,mdc,nframes);
			break;
		case '2':
			hell_sqr(out,mdc,nframes);
			break;
		case '3':
			psychedelic_if(out,mdc,nframes);
			break;
		case '4':
			by_60s(out,mdc,nframes);
			break;
		case '5':
			fuzzy_dark_pow4(out,mdc,nframes);
			break;
		case '6':
			rare_cuadratic(out,mdc,nframes);
	}*/
	mod2 = rand()%7;
	if(mod2 == 0) 			log_rock(out,mdc,nframes);
	if(mod2 == 1)			log_rock2(out,mdc,nframes);
	if(mod2 == 2)			hell_sqr(out,mdc,nframes);
	if(mod2 == 3)			psychedelic_if(out,mdc,nframes);
	if(mod2 == 4)			by_60s(out,mdc,nframes);
	if(mod2 == 5)			fuzzy_dark_pow4(out,mdc,nframes);
	if(mod2 == 6)			rare_cuadratic(out,mdc,nframes);
}	

