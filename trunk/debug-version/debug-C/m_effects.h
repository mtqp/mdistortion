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
EFFECT dummy_func;

EFFECT * f_effect[4];
EFFECT * equalizer_effect;
EFFECT * delay_effect;
EFFECT * hall_effect;

void init_m_effects(struct _m_distortion* md);
void eq_new(struct _m_distortion* md);

#endif
