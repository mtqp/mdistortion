#ifndef __DELAY_H__
#define __DELAY_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct _m_delay {
	int    dl_size;
	int    dl_sub_i;
	int	   dl_cant_bufs_active;	//1,2,3 o 4
	int	   dl_total_bufs;
	int    dl_speed;			//algo cercano a [65536 ... 262144] multiplo de 4096
	float**dl_bufs;
} m_delay;

m_delay* delay_new (int size, int total);
#endif
