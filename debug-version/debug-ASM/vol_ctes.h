#ifndef __VOL_CTES_H__
#define __VOL_CTES_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct _vol_ctes {
	float log_rock_v[4];
	float log_rock2_v[4];
	float hell_sqr_v[4];
	float psyche_v[4];
	float by_60s_v[4];
	float fuzzy_dark_v[4];
	float rare_c_v[4];
} vol_ctes;

void set_vol_ctes (vol_ctes* v);
vol_ctes* vol_new();
#endif
