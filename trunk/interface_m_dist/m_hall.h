#ifndef __HALL_H__
#define __HALL_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct _m_hall {
	int    hll_size;
	float  hll_coef1;
	float  hll_coef2;
	float  hll_coef3;
	float  hll_coef4;		//valores entre 0..1 coeficientes de los buffs... pensar si no puede ser uno solo
	float* hll_buf1;
	float* hll_buf2;
	float* hll_buf3;
	float* hll_buf4;
} m_hall;

m_hall* hall_new(int size);

#endif
