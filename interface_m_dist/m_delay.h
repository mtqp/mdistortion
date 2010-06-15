#ifndef __DELAY_H__
#define __DELAY_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct _m_delay {
	int    dl_size;
	int    dl_sub_i;
	float* dl_buf1;
	float* dl_buf2;
	float* dl_buf3;
} m_delay;

m_delay* delay_new (int size);
#endif
