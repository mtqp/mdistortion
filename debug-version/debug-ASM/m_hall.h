#ifndef __HALL_H__
#define __HALL_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct _m_hall {
	int    hll_size_bufs;
	float  hll_coef;
	int  hll_buf_quantity;		
	float**hll_bufs;
} m_hall;				//Estructura HALL

m_hall* hall_new(int size, int quantity);

#endif
