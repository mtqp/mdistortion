#ifndef __M_DELAY_H__
#define __M_DELAY_H__

#include "m_distortion.h"
#include <jack/jack.h>

typedef struct _m_delay {
//	speaker _s_delayed;
	jack_default_audio_sample_t *_delay_buf;	
	jack_nframes_t _buf_size;
	bool _occupied_buf;
} m_delay;

void init_m_delay(m_delay *mdelay, /*speaker delayed,*/ jack_nframes_t nframes);
void free_m_delay(m_delay *mdelay);
void delaying(m_distortion *md, m_delay *delay, jack_default_audio_sample_t *outL, jack_default_audio_sample_t *outR, jack_nframes_t nframes);

#endif
