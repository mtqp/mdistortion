#ifndef __GLOBALS_H__
#define __GLOBALS_H__
#include <jack/jack.h>
#include <stdio.h>
#include "m_distortion.h"

#define sensitivo 1
#define no_sensitivo 0
#define def_toggled 1
#define not_def_toggled 0
#define back_to_rock_mode -1

jack_port_t *input_port;
jack_port_t *output_left;
jack_port_t *output_right;

struct _m_distortion *m_dist; 

typedef struct _globals {
	int _eq_sensitive; 
	int _chorus_toggled;
	int _delay_toggled;

	//FILE *f_out;
	int plot_x;
	int plot_by_pass;
} globals;

globals* global_ptr;
#endif