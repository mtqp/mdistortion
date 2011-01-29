#ifndef __M_EFFECTS_H__
#define __M_EFFECTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "m_distortion.h"
#include "m_eq.h"
#include "m_delay.h"
#include "m_hall.h"

//version para C --> typedef float (EFFECT) (struct _m_distortion *md, float sample, int i);
/*version ASM*/
typedef float (EFFECT) (struct _m_distortion *md, int i);

extern EFFECT equalizer_func;
extern EFFECT delay_func;
extern EFFECT hall_func;
extern EFFECT dummy_func;
//EFFECT volume_func;

EFFECT * f_effect[5];
EFFECT * equalizer_effect;
EFFECT * delay_effect;
EFFECT * hall_effect;
EFFECT * volume_effect; //todavia volume no se usa para nada

void init_m_effects(struct _m_distortion* md);
void eq_new(struct _m_distortion* md);

/*extern void hall_asm(struct _m_distortion* md, int buf_size);
extern void dummy_asm(struct _m_distortion* md, int buf_size);**/

#endif
