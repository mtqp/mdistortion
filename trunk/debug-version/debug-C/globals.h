#ifndef __GLOBALS_H__
#define __GLOBALS_H__
#include <stdio.h>
#include "m_distortion.h"
#include "m_user_interface.h"
#include "dummy_audio.h"

#define sensitivo 1
#define no_sensitivo 0
#define def_toggled 1
#define not_def_toggled 0
#define back_to_rock_mode -1

/*Variables globales que utiliza todo el programa*/

struct _m_distortion 	 *m_dist; 
struct _m_user_interface *m_ui;
dummy_audio*	dum_audio;
/*typedef struct _globals {
	//FILE *f_out;
	int plot_x;
	int plot_by_pass;
} globals;

globals* global_ptr;*/
#endif
