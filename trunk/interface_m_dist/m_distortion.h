#ifndef __M_DISTORTIOM_H__
#define __M_DISTORTIOM_H__

#include <jack/jack.h>
#include "globals.h"
#include "m_eq.h"
#include "vol_ctes.h"

typedef struct _m_distortion {
	unsigned int _d_active;
	unsigned int _last_dist_active;		//para q esta esto!?!?
	
	m_equalizer* m_bass;
	m_equalizer* m_treb;
		
	float _dvol;
	vol_ctes * _vctes;
	
	float _dgain;
	float _variacion_vol;
	float _variacion_gain;
	
	unsigned int _cant_distors;
	char* _name_dists[10];
}m_distortion;


void init_m_distortion(m_distortion * md);
void free_m_distortion(m_distortion *md);
void distortionize(m_distortion *md, jack_default_audio_sample_t *out, jack_nframes_t nframes);
void set_m_distortion( m_distortion * md, int n);//name_dists *n/*, unsigned char right*/);

typedef void (DISTORTION)(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes);

DISTORTION log_rock;
DISTORTION log_rock2;
DISTORTION hell_sqr;
DISTORTION psychedelic_if;
DISTORTION by_60s;
DISTORTION fuzzy_dark_pow4;
DISTORTION rare_cuadratic;
DISTORTION random_day;
DISTORTION mute;
DISTORTION by_pass;

DISTORTION * f_dist[10];	
DISTORTION * distortion_channel;

//extern volume_hell_sqr(jack_default_audio_sample_t *out, m_distortion *mdc, jack_nframes_t nframes);

float actual_gain (m_distortion *mdc);
void gain_up(m_distortion *mdc);
void gain_down(m_distortion *mdc);

#endif
