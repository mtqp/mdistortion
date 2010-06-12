
#include "vol_ctes.h"

void set_vol_ctes (vol_ctes* v){
	/*facilita el calculo tenerlas en memoria*/
	v->log_rock_v 	=  0.250; 
	v->log_rock2_v	=  0.500;
	v->hell_sqr_v	=  0.015;
	v->pyche_v		= -1.000;
	v->by_60s_v		= -1.000;
	v->fuzzy_dark_v	=  0.0125;
	v->rare_c_v		= -1.000;
	printf("	Volume Set\n");
}

void vol_new(vol_ctes* v){
	v = (vol_ctes*) malloc(sizeof(vol_ctes));
	if(v!=NULL){
		set_vol_ctes(v);
	} else {
		printf("Couldn't malloc Volume Structure\n");
	}
}
