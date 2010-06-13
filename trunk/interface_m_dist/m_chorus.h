#ifndef __CHORUS_H__
#define __CHORUS_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct _m_chorus {
	int chr_size;
	float* chr_buf1;
	float* chr_buf2;
	float* chr_buf3;
	float* chr_buf4;	//es muy posible q se pueda hacer con un solo buffer pisando las cosas, pensar...
} m_chorus;

m_chorus* chorus_new(int size);

#endif
