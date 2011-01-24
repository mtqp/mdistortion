
#include "vol_ctes.h"

/*Inicializa estructura VOL_CTES*/
vol_ctes* vol_new(){
	vol_ctes* v = (vol_ctes*) malloc(sizeof(vol_ctes));
	if(v!=NULL){
		set_vol_ctes(v);
		printf("vol * value = %d\n",(int) v);
		return v;
	} else {
		printf("Couldn't malloc Volume Structure\n");
		return NULL;
	}
}

void set_vol_ctes (vol_ctes* v){
	/*facilita el calculo tenerlas en memoria*/
	int i = 0;
	for(i;i<4;i++){
		v->log_rock_v[i]	= 0.250;
		v->log_rock2_v[i]	= 0.500;
		v->hell_sqr_v[i]	= 0.015;
		v->psyche_v[i]		= -1.000;
		v->by_60s_v[i]		= -1.000;
		v->fuzzy_dark_v[i]	= 0.0125;
		v->rare_c_v[i]		= -1.000;
	}
	printf("	Volume Set\n");
}

