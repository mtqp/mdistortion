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
	d->_variacion_vol  = var_vol;
	d->_variacion_gain = var_gain;

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
	printf("m_distortion_channel initialized\n");
}

float actual_volume (m_distortion_channel *mdc){
	return mdc->_dvol;
}

void volume_up(m_distortion_channel *mdc){
	mdc->_dvol -= mdc-> _variacion_vol;	///if no alcanzo el max, ver como corregir eso
}


void volume_down(m_distortion_channel *mdc){
		mdc->_dvol += mdc ->_variacion_vol;
}

float actual_gain (m_distortion_channel *mdc){
	return mdc->_dgain;
}

void gain_up(m_distortion_channel *mdc){
	mdc->_dgain -= mdc-> _variacion_gain;	///if no alcanzo el max, ver como corregir eso
}


void gain_down(m_distortion_channel *mdc){		//notar q no hay problema, el metodo distor se encarga de elevar⁴ este parametro tanto como en volumen
		mdc->_dgain += mdc ->_variacion_gain;
}

/////////////////////////////////////////////////////
///----------------DISTORSIONES-------------------///
/////////////////////////////////////////////////////
///LAS CTES SE ENCARGAN DE ESTABILIZAR TODOS EN MISMO VOLUMENES, Y CON DISTORS COPADAS

void log_rock(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//logaritmic rock //seno hace mas agudo// cos mas grave
	int i = 0;
	for(i;i<nframes;i++){
		out[i]=(sin(cos(log(sin(log(out[i]))))))/mdc->_dvol; /// ENTRA CON EL VOL MAX Q TIENE
	}
}

void log_rock2(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= cos(tan(tan(log((out[i])))))/mdc->_dvol;//todavia se puede poner un poquito ams fuerte pero nada mas
		//tan lo hace mas grave sintbm, pero menos q tan ///-sen mas grave todavia
	}
}

void hell_sqr(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//raiz nasty
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= (1000.0*sqrt(out[i]))/pow(mdc->_dvol,3);
	}
}

void psychedelic_if(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //arco tangente
	int i = 0;
	for(i;i<nframes;i++){
		if(i < nframes/3) {
			out[i] = (log(out[i])*10000.0)/(5*pow(mdc->_dvol,5));
		} else {
			out[i] = sin(log(sin(out[i])))/pow(mdc->_dvol,5);
		}
	}
}

void by_60s(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= (100.0 * out[i])/mdc->_dgain; ///funciona de volumen-ganancia, fijarse q se puede hacer
	}
}

void fuzzy_dark_pow4(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //fuzzy oscura // siempre negativa.
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= (100000000.0*(-pow(out[i],4)))/pow(mdc->_dvol,3); //mas bien mutea la guitarra
	}
}

void rare_cuadratic(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //cuadratica RARISIMA
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= (11000.0*(pow(out[i],2)))/pow(mdc->_dgain,2);
	}
}

void random_day(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){
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

void mute(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){
	int i=0;
	for(i;i<nframes;i++){
		out[i] = 0.0;
	}	
}

void by_pass(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){
	//mmm no hago nada?
}
