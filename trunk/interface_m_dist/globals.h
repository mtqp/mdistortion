#include <jack/jack.h>
#include "m_distortion.h"


jack_port_t *input_port;
jack_port_t *output_left;
jack_port_t *output_right;

m_distortion *m_dist; 

int global_eq_sensitive; 
float dt;
float RC;
float alpha;

float limpio_i_menos_uno;
