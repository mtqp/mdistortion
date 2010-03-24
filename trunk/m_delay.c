#include "m_distortion.h"
//#include "m_distortion_ch.h"
#include <jack/jack.h>
#include <stdio.h>
#include <stdlib.h>

//ESTA FUNCIONANDO SOLO EN IZQUIERDO DELAYEDder Y NO ADMITE DELAY CON OTRAS DISTORSIONES.

void init_m_delay(m_delay *mdelay, /*speaker delayed,*/ jack_nframes_t nframes){
//	mdelay->_s_delayed = delayed;
	mdelay->_buf_size  = nframes;
	mdelay->_delay_buf = (jack_default_audio_sample_t *) malloc(mdelay->_buf_size);
	mdelay->_occupied_buf = false;
}

void free_m_delay(m_delay *mdelay){
	free(mdelay->_delay_buf);
	free(mdelay);
}

void delaying(m_distortion *md, m_delay *delay,jack_default_audio_sample_t *outL, jack_default_audio_sample_t *outR, jack_nframes_t nframes){
	distorsion_left(outL,md->_s_left,nframes);
	if(!delay->_occupied_buf){
		memcpy(delay->_delay_buf,outL,delay->_buf_size);
	} else {
		memcpy(outR,delay->_delay_buf,delay->_buf_size);				
	}
	delay->_occupied_buf = !delay->_occupied_buf;
}
