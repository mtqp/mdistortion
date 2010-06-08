#ifndef __GLOBALS_H__
#define __GLOBALS_H__
#include <jack/jack.h>
#include "m_distortion.h"
#include <stdio.h>

jack_port_t *input_port;
jack_port_t *output_left;
jack_port_t *output_right;

struct _m_distortion *m_dist; 

typedef struct _globals {

	int _eq_sensitive; 
	int _noise_toggled;
	
	float dt;
	float RC;
	float alpha;

	float limpio_i_menos_uno;

	//FILE *f_out;
	int plot_x;
	int plot_by_pass;

} globals;

globals* global_ptr;

#endif
