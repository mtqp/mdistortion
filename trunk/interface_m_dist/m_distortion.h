#ifndef __M_DISTORTIOM_H__
#define __M_DISTORTIOM_H__

#include <jack/jack.h>
#include "globals.h"
#include "m_eq.h"
#include "vol_ctes.h"

typedef struct _m_distortion {
	int _d_active;
	int _last_dist_active;
	
	m_equalizer* m_bass;
		
	float _dvol;		//se usa
	vol_ctes * _vctes;	//si cambio de lugar la estructura aca jodo el ASM

	m_equalizer* m_treb;
	int _rock_mode_left;	
		
	float _dgain;		//no se usa todavia
	float _variacion_vol;//tmp se usa todavia
	float _variacion_gain;//tmp se usa todavia
	
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
