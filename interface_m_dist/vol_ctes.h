#ifndef __VOL_CTES_H__
#define __VOL_CTES_H__
typedef struct _vol_ctes {
	/*static*/ float log_rock_v;
	 float log_rock2_v;
	 float hell_sqr_v;
	 float pyche_v;
	 float by_60s_v;
	 float fuzzy_dark_v;
	 float rare_c_v;
} vol_ctes;

void set_vol_ctes (vol_ctes* v);

#endif
