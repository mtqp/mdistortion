#include "m_distortion.h"
#include "m_distortion_ch.h"
#include <jack/jack.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _m_delay {
//	speaker _s_delayed;
	jack_default_audio_sample_t *_delay_buf;	
	jack_nframes_t _buf_size;
	bool _occupied_buf
} m_delay;

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

void delaying(m_distortion *md, jack_default_audio_sample_t *outL, jack_default_audio_sample_t *outR, jack_nframes_t nframes){


}
