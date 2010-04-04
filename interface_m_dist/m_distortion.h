#ifndef __M_DISTORTIOM_H__
#define __M_DISTORTIOM_H__

#include <jack/jack.h>
#include "m_distortion_ch.h"

/*
enum name_dists {
	e_log_rock 			= 0,
	e_log_rock_II	 	= 1,
	e_hell_sqrt 		= 2,
	e_psychedelic_if 	= 3,
	e_by_60s 			= 4,
	e_fuzzy_dark_pow_IV = 5,
	e_rare_cuadratic 	= 6,
	e_random_day 		= 7,
	e_mute 				= 8,
	e_by_pass 			= 9,
};
*/

typedef struct _m_distortion {
	m_distortion_channel *_d_ch;
	unsigned char _d_left;
	unsigned char _last_dist_active;
	unsigned char _cant_distors;
	char* _name_dists[10];
}m_distortion;


DISTORTION *distorsion_channel;

void init_m_distortion(m_distortion * md);
void free_m_distortion(m_distortion *md);
void distortionize(m_distortion *md, jack_default_audio_sample_t *outL, jack_default_audio_sample_t *outR, jack_nframes_t nframes);
void set_m_distortion( m_distortion * md, int n);//name_dists *n/*, unsigned char right*/);

/*
void vol_up_md (m_distortion *md, speaker sp);
void vol_down_md (m_distortion *md, speaker sp);
void gain_up_md (m_distortion *md, speaker sp);
void gain_down_md (m_distortion *md, speaker sp);
void reset_gain(m_distortion *md, speaker sp);
void reset_vol(m_distortion *md, speaker sp);
*/
#endif
