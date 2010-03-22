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
	//d->_nframes = nframes;

	//SETEAR TODAS LAS DISTORSIONES! EN EL ARRAY MALDITO INT! :d:d:d:d:
	f_dist[0] = &sweet_arc_tan;
	f_dist[1] = &log_rock;
	f_dist[2] = &log_rock2;
	f_dist[3] = &hell_sqr;
	f_dist[4] = &raw_sum2i;
	f_dist[5] = &rare_cuadratic;
	f_dist[6] = &fuzzy_dark_pow4;
	f_dist[7] = &by_pass;
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
////////////////////////////////////////////////////

////PENSAR EN Q LAS GANACIAS Y VOLUMENES VAN A TENER Q ESTAR ESTABILIZADOS PARA TODOS X IGUAL DESDE LAS FUNCIONES
void rare_cuadratic(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //cuadratica RARISIMA
//	printf("rare_cuadratic\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= (mdc->_pow_dvol+10.0)*(pow(out[i],2)/*/(mdc->_pow_dgain)*/)*10.0;
	}
}


void raw_sum2i(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //sum(d/2^i) bastante normal
	printf("raw sum 2i\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= (out[i] + out[i]/2 + out[i]/4 + out[i]/8 + out[i]/16 + out[i]/32 /*+ out[i]/64 + out[i]/128*/)*(mdc->_pow_dvol); ///sera vol o gain???
	}
}

void fuzzy_dark_pow4(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //fuzzy oscura // siempre negativa.
	printf("fuzzy dark pow4\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= 800000.0*(-pow(out[i],4))*(mdc->_pow_dvol);
	}
}

void log_rock(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//logaritmic rock //seno hace mas agudo// cos mas grave
	printf("log rock\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]=mdc->_pow_dvol*(log(out[i]*(mdc->_pow_dgain)));//con seno tbm qda RE buena
	}
}

void log_rock2(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//
	printf("log rock II\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= log(sin(out[i]))/(cos(-(mdc->_pow_dgain)));//tan lo hace mas grave sintbm, pero menos q tan ///-sen mas grave todavia
	}
}

void hell_sqr(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//raiz nasty
	printf("hell sqrt\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= 1000.0*(mdc->_pow_dvol)*sqrt(out[i]); ///se puede elevar hasta el cuadrado data y suena copado
	}
}

void sweet_arc_tan(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //arco tangente
	printf("sweet_arc_tan\n");
	int i = 0;
	float vol = (mdc->_pow_dvol)/5;
	for(i;i<nframes;i++){
		out[i] = vol * atan(out[i]*(mdc->_pow_dgain));	 	//q la mejor aproximacion es la x4
	}
}


void by_pass(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){
	printf("by pass\n");
	int i = 0;
	for(i;i<nframes;i++){
		out[i]= 100.0*mdc->_pow_dvol * out[i];
	}
}

