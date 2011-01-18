#ifndef __M_DISTORTIOM_H__
#define __M_DISTORTIOM_H__

struct _m_distortion;

//#include <jack/jack.h>
#include "dummy_audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "globals.h"
#include "m_effects.h"
#include "m_eq.h"
#include "m_hall.h"
#include "m_delay.h"
#include "vol_ctes.h"
#include "enum_dist.h"
#include "enum_effects.h"

typedef struct _m_distortion {
	int _last_dist_active;
	
	float _dvol;
	vol_ctes * _vctes;

	m_equalizer* m_bass;
	m_equalizer* m_treb;
	m_equalizer* m_mid;
	
	m_delay*  	_delay;
	m_hall* 	_hall;

}m_distortion;			//estructura principal


void init_m_distortion(m_distortion * md);
void free_m_distortion_and_effects(m_distortion *md);
void distortionize(m_distortion *md, dummy_audio_buf out, dummy_audio_size nframes);
void set_m_distortion( m_distortion * md, int distor);

typedef void (DISTORTION)(dummy_audio_buf out, m_distortion *mdc, dummy_audio_size nframes);

extern DISTORTION log_rock;
extern DISTORTION log_rock2;
extern DISTORTION hell_sqr;
DISTORTION psychedelic_if;
DISTORTION by_60s;
DISTORTION fuzzy_dark_pow4;
DISTORTION rare_cuadratic;
DISTORTION random_day;
DISTORTION mute;
DISTORTION by_pass;

DISTORTION * f_dist[10];	
DISTORTION * distortion_channel;

#endif
