
#include "m_distortion_ch.h"
#include "m_distortion.h"
#include <jack/jack.h>
#include <stdio.h>

void init_m_distortion(m_distortion * md, master_type master){
	float vol = 1.0;
	float gain = 10.0;
	float var = 0.5;
	init_m_distortion_ch(md->_s_left, vol, gain, var, var);
	init_m_distortion_ch(md->_s_rigth, vol, gain ,var ,var);
	md->_master_ch = master;
	md->_d_left = 7;			//comienzan en by_pass
	md->_d_right= 7;
	md->_cant_distors = 8;
/*	distorsion_left  =	(md->_s_left)->f_dist[md->_d_left]; 
	distorsion_right =  (md->_s_right)->f_dist[md->_d_right];
*/	
	distorsion_left  =	f_dist[md->_d_left]; 
	distorsion_right =  f_dist[md->_d_right];
}

void distortionize(m_distortion *md, jack_default_audio_sample_t *outL, jack_default_audio_sample_t *outR, jack_nframes_t nframes){
	distorsion_left(outL, md->_s_left, nframes);
	distorsion_right(outR, md->_s_right, nframes);
}

void reset_master (m_distortion *md, master_type master){
	md->_master_ch = master;
}

void set_m_distortion( m_distortion * md, unsigned char left, unsigned char right){
	if (md->_master_ch == master_off){		//esto quizas es redundante ya q master es un bool
		if(!(left >= md->_cant_distors || right >= md->_cant_distors)){
			md->_d_left = left;
			md->_d_right= right;
			distorsion_left = f_dist[md->_d_left];		///es necesarioVOLVER A SETEARLA SI LE PASO LA MISMA DISTOR?
			distorsion_right =  f_dist[md->_d_right];
		} else {
			printf("el numero de distorsion pedido, excede la cantidad total de distorsiones \n");
		}
	} else {
		printf ("master on, ambos canales se setearan con el valor de left\n");
		md->_d_left = left;
		md->_d_right= left;
		distorsion_left = f_dist[md->_d_left];
		distorsion_right =  f_dist[md->_d_right];
	}
}

void vol_up_md (m_distortion *md, speaker sp){
	if (md->_master_ch == master_on){		//esto quizas es redundante ya q master es un bool
		volume_up(md->_s_left);
		volume_up(md->_s_right);
		printf("setting volume for both channels");
	} else {
		if(sp){
			volume_up(md->_s_left);
		} else {
			volume_up(md->_s_right);
		}
	}
}

void vol_down_md (m_distortion *md, speaker sp){
	if (md->_master_ch){		//x ser  redundante ya q master es un bool no agrego comparacion
		volume_down(md->_s_left);
		volume_down(md->_s_right);
		printf("setting volume for both channels");
	} else {
		if(sp){
			volume_down(md->_s_left);
		} else {
			volume_down(md->_s_right);
		}
	}
}

void gain_up_md (m_distortion *md, speaker sp){
	if (md->_master_ch){
		gain_up(md->_s_left);
		gain_up(md->_s_right);
		printf("setting gain for both channels");
	} else {
		if(sp){
			gain_up(md->_s_left);
		} else {
			gain_up(md->_s_right);
		}
	}
}

void gain_down_md (m_distortion *md, speaker sp){
	if (md->_master_ch){
		gain_down(md->_s_left);
		gain_down(md->_s_right);
		printf("setting gain for both channels");
	} else {
		if(sp){
			gain_down(md->_s_left);
		} else {
			gain_down(md->_s_right);
		}
	}
}

