#include <stdio.h>
#include <stdlib.h>
#include "m_distortion_ch.h"
//#include "m_distortion.h"
#include <jack/jack.h>

//DESPUES LE VAMOS A TENER Q AGREGAR EL EQ!


void init_m_distortion_ch(m_distortion_channel *d, float vol, float gain, float var_vol, float var_gain, /*jack_nframes_t nframes*/ ){
	d->_dvol = vol;
	d->_dgain= gain;
	d->_pow_dvol = pow(vol,4);
	d->_pow_dgain= pow(gain,4);
	d->_variacion_vol  = var_vol;
	d->_variacion_gain = var_gain;
	//d->_nframes = nframes;

	//SETEAR TODAS LAS DISTORSIONES! EN EL ARRAY MALDITO INT! :d:d:d:d:
	f_dist[0] = &sweet_arc_tan;
	f_dist[1] = &rare_cuadratic;
	f_dist[2] = &raw_sum2i;
	f_dist[3] = &fuzzy_dark_pow4;
	f_dist[4] = &log_rock;
	f_dist[5] = &log_rock2;
	f_dist[6] = &hell_sqr;
	f_dist[7] = &by_pass;
}

////PENSAR EN Q LAS GANACIAS Y VOLUMENES VAN A TENER Q ESTAR ESTABILIZADOS PARA TODOS X IGUAL DESDE LAS FUNCIONES
void sweet_arc_tan(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //arco tangente
	int i = 0;
	for(i;i<nframes;i++){
		_out[i] = (mdc->_pow_dvol) * atan(_out[i]*(mdc->_pow_dgain));	 	//q la mejor aproximacion es la x4
	}
}

void rare_cuadratic(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //cuadratica RARISIMA
	int i = 0;
	for(i;i<nframes;i++){
		_out[i]= (mdc->_pow_dvol)*(pow(_out[i],2))*(mcd->_pow_dgain);
	}
}


void raw_sum2i(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //sum(d/2^i) bastante normal
	int i = 0;
	for(i;i<nframes;i++){
		_out[i]= (_out[i] + _out[i]/2 + _out[i]/4 + _out[i]/8 + _out[i]/16 + _out[i]/32 + _out[i]/64 + _out[i]/128)*(mdc->_pow_dgain); ///sera vol o gain???
	}
}

void fuzzy_dark_pow4(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){ //fuzzy oscura // siempre negativa.
	int i = 0;
	for(i;i<nframes;i++){
		_out[i]= (-pow(_out[i],4))*(mdc->_pow_dvol);
	}
}

void log_rock(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//logaritmic rock //seno hace mas agudo// cos mas grave
	int i = 0;
	for(i;i<nframes;i++){
		_out[i]=mdc->_pow_dvol*(log(_out[i]*(mdc->_pow_dgain)));//con seno tbm qda RE buena
	}
}

void log_rock2(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//
	int i = 0;
	for(i;i<nframes;i++){
		_out[i]= log(sin(_out[i]))/(cos(-(mdc->_pow_dgain)));//tan lo hace mas grave sintbm, pero menos q tan ///-sen mas grave todavia
	}
}

void hell_sqr(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){//raiz nasty
	int i = 0;
	for(i;i<nframes;i++){
		_out[i]= (mdc->_pow_dvol)*sqrt(_out[i]); ///se puede elevar hasta el cuadrado data y suena copado
	}
}

void by_pass(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes){
	int i = 0;
	for(i;i<nframes;i++){
		_out[i]=mdc->_pow_dvol * _out[i];
	}
}

float actual_volume (m_distortion_channel *mdc){
	return mdc->_dvol;
}

void volume_up(m_distortion_channel *mdc){
	mdc->dvol += mdv-> _variacion_vol;	///if no alcanzo el max, ver como corregir eso
	mdc->_pow_dvol = pow(mdc->_dvol,4);
}


void volume_down(m_distortion_channel *mdc){
	if(mdc->_dvol > mdc->_variacion_vol){
		mdc->_dvol -= mdc ->_variacion_vol;
		mdc->_pow_dvol = pow(mdc->_dvol,4);
	} else {
		mdc->_dvol = 0.0;
	}
}

float actual_gain (m_distortion_channel *mdc){
	return mdc->_dgain;
}

void gain_up(m_distortion_channel *mdc){
	mdc->dgain += mdv-> _variacion_gain;	///if no alcanzo el max, ver como corregir eso
	mdc->_pow_dgain = pow(mdc->_dgain,4);
}


void gain_down(m_distortion_channel *mdc){		//notar q no hay problema, el metodo distor se encarga de elevar⁴ este parametro tanto como en volumen
	if(mdc->_dgaib > mdc->_variacion_gain){
		mdc->_dgain -= mdc ->_variacion_gain;
		mdc->_pow_dgain = pow(mdc->_dgain,4);
	} else {
		mdc->_dgain = 0.0;
	}
}

int main(){
	while (1){
		printf ("fuck you\n");
	}
	return 0;
}

