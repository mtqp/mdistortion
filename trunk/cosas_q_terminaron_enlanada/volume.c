#ifndef __VOLUME_c__
#define __VOLUME_C__

/*typedef int bool;
typedef 0 left;
typedef 1 right;
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct _volume {
	float 	_vol_left;
	float 	_vol_right;
	float 	_variacion_vol;
	int 	_master_vol;
}volume;

void init_volume(volume *vol, int master,float variacion){
	vol->_vol_left 	= 3.0; //(sera el volumen medio??? eso con el tiempo hay q indagarlo)
	vol->_vol_right = 3.0;
	vol->_master_vol= master;
	vol->_variacion_vol = variacion; // el parametro de entrada variacion... pensar y probar
}

float actual_volume (volume *vol){
	if (vol->_master_vol){
		return vol->_vol_right;
	} else {
		printf ("no hay volumen maestro.\n");
		return -1.0;
	}
}

void volume_up(volume *vol){
	if (vol->_master_vol){
		vol->_vol_left	+=	vol->_variacion_vol;		// hay q ver en q punto satura para no hacerlo aumentar mas.
		vol->_vol_right	+=	vol->_variacion_vol;
	} else {
		printf("no hay volumen maestro\n");
	}
}

void volume_up_speaker(volume *vol, int speaker){
	if(!vol->_master_vol){
		if(/*left ==*/ speaker){
			vol->_vol_left 	+=	vol->_variacion_vol;
		} else {
			vol->_vol_right += 	vol->_variacion_vol;
		}
	} else {
		printf ("funcionando en master vol\n");
	}
}

void volume_down(volume *vol){
	if(vol->_vol_left > vol->_variacion_vol){
		vol->_vol_left -= vol->_variacion_vol;
		vol->_vol_right = vol->_vol_left;
	} else {
		vol->_vol_left = 0.0;
		vol->_vol_right = 0.0;
	}
}

void volume_down_speaker(volume *vol, int speaker){
	if(!vol->_master_vol){
		if(/*left ==*/ speaker){
			vol->_vol_left 	-=	vol->_variacion_vol;
		} else {
			vol->_vol_right -= 	vol->_variacion_vol;
		}
	} else {
		printf ("funcionando en master vol\n");
	}	
}

#endif

