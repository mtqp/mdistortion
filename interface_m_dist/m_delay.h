#ifndef __DELAY_H__
#define __DELAY_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct _m_delay {
	int    dl_size;
	int    dl_sub_i;
	int	   dl_cant_bufs_active;	//1,2,3 o 4 + modo extremo (todos los buffers, en particular este programa usa 8)
	int	   dl_total_bufs;		//cantidad total
	int    dl_speed;			//[32768 ... 262144] multiplo de 4096
	float**dl_bufs;
} m_delay;

m_delay* delay_new (int size, int total);
#endif
