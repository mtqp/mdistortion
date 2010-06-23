#ifndef __VOL_CTES_H__
#define __VOL_CTES_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct _vol_ctes {
	float log_rock_v;
	float log_rock2_v;
	float hell_sqr_v;
	float psyche_v;
	float by_60s_v;
	float fuzzy_dark_v;
	float rare_c_v;
} vol_ctes;

void set_vol_ctes (vol_ctes* v);
vol_ctes* vol_new();
#endif
