#ifndef __M_DISTORTIOM_H__
#define __M_DISTORTIOM_H__

#include <jack/jack.h>
#include "m_distortion_ch.h"
#ifndef bool
    #define bool unsigned char
    #define true 1
    #define false 0
#endif

#ifndef speaker
    #define speaker unsigned char
    #define speaker_izq 1
    #define speaker_der 0
#endif

#ifndef master_type
    #define master_type bool
    #define master_on true
    #define master_off false
#endif


typedef struct _m_distortion {
	m_distortion_channel *_s_left;
	m_distortion_channel *_s_right;
//	m_delay *_delay;
	master_type _master_ch;
	unsigned char _d_left;
	unsigned char _d_right;
	unsigned char _cant_distors;
	char* _name_dists[8];
}m_distortion;


DISTORTION *distorsion_left;
DISTORTION *distorsion_right;

void init_m_distortion(m_distortion * md, master_type master);
void free_m_distortion(m_distortion *md);
void distortionize(m_distortion *md, jack_default_audio_sample_t *outL, jack_default_audio_sample_t *outR, jack_nframes_t nframes);
void change_master (m_distortion *md);
void set_m_distortion( m_distortion * md, unsigned char left, unsigned char right);
void vol_up_md (m_distortion *md, speaker sp);
void vol_down_md (m_distortion *md, speaker sp);
void gain_up_md (m_distortion *md, speaker sp);
void gain_down_md (m_distortion *md, speaker sp);
void reset_gain(m_distortion *md, speaker sp);
void reset_vol(m_distortion *md, speaker sp);

#endif
