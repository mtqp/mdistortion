#ifndef __M_EFFECTS_H__
#define __M_EFFECTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "m_distortion.h"
#include "m_eq.h"
#include "m_delay.h"
#include "m_hall.h"

typedef float (EFFECT) (struct _m_distortion *md, float sample, int i);

EFFECT equalizer_func;
EFFECT delay_func;
EFFECT hall_func;
EFFECT volume_func;
EFFECT dummy_func;

EFFECT * f_effect[5];
EFFECT * equalizer_effect;
EFFECT * delay_effect;
EFFECT * hall_effect;
EFFECT * volume_effect; //todavia volume no se usa para nada

void init_m_effects(struct _m_distortion* md);
void eq_new(struct _m_distortion* md);

extern void hall_asm(struct _m_distortion* md, int buf_size);
extern void dummy_asm(struct _m_distortion* md, int buf_size);

#endif
