#ifndef __M_DISTORTION_H__
#define __M_DISTORTION_H__

#include <jack/jack.h>

typedef struct _m_distortion_channel {
	float _dvol;
	float _dgain;
	float _pow_dvol;
	float _pow_dgain;
	float _variacion_vol;
	float _variacion_gain;
//	jack_nframes_t _nframes;
}m_distortion_channel;

typedef void (DISTORTION)(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes);

DISTORTION sweet_arc_tan;
DISTORTION rare_cuadratic;
DISTORTION raw_sum2i;
DISTORTION fuzzy_dark_pow4;
DISTORTION log_rock;
DISTORTION log_rock2;
DISTORTION hell_sqr;
DISTORTION by_pass;

DISTORTION * f_dist[8];			///esto dependera de la cantidad de distorsiones finales q tenga.

//function settings
void init_m_distortion_ch(m_distortion_channel *d, float vol, float gain, float var_vol, float var_gain);
float actual_volume (m_distortion_channel *mdc);			//quizas estas dos no las use nunca
float actual_gain (m_distortion_channel *mdc);
void volume_up(m_distortion_channel *mdc);
void volume_down(m_distortion_channel *mdc);
void gain_up(m_distortion_channel *mdc);
void gain_down(m_distortion_channel *mdc);


/*//function distorting
void sweet_arc_tan(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes);
void rare_cuadratic(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes);
void raw_sum2i(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes);
void fuzzy_dark_pow4(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes);
void log_rock(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes);
void log_rock2(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes);
void hell_sqr(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes);
void by_pass(jack_default_audio_sample_t *out, m_distortion_channel *mdc, jack_nframes_t nframes);
*/


#endif
