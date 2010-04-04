#include "m_distortion.h"

#include <jack/jack.h>
#include <stdio.h>
#include <stdlib.h>

void init_m_distortion(m_distortion * md){
	printf("inicializando m_distortion\n");
	float vol = 1.0;
	float gain = 1.0;
	float var = 1.0;
	md->_d_ch = (m_distortion_channel *) malloc(sizeof(m_distortion_channel));
	init_m_distortion_ch(md->_d_ch, vol, gain, var, var);

	md->_d_left = 2;			
	md->_last_dist_active = md->_d_left;
	md->_cant_distors 	= 10;
	distorsion_channel  = f_dist[md->_d_left]; 

	md->_name_dists[0] = "log_rock";
	md->_name_dists[1] = "log_rockII";
	md->_name_dists[2] = "hell sqr";
	md->_name_dists[3] = "psychedelic_if";
	md->_name_dists[4] = "by_60s";
	md->_name_dists[5] = "fuzzy_dark_pow4";
	md->_name_dists[6] = "rare_cuadratic";
	md->_name_dists[7] = "random_day";
	md->_name_dists[8] = "mute";
	md->_name_dists[9] = "by_pass";
	printf("\n\nm_distortion inicializada\n\n");
}

void free_m_distortion(m_distortion *md){
	free (md->_d_ch);
	free (md);
	printf("freeing m_distortion exitoso\n");
}

void distortionize(m_distortion *md, jack_default_audio_sample_t *out, jack_nframes_t nframes){
	distorsion_channel(out, md->_d_ch, nframes);

}

/*
void change_master (m_distortion *md){
	if(md->_master_ch){
		printf("setting to master off\n");
		md->_master_ch = master_off;
	} else {
		printf("setting to master on\n");
		md->_master_ch = master_on;
		set_m_distortion(md,md->_d_left,0);			//el cero esta de mas, xq va a setear la distor izquierda
	}

}
*/

void set_m_distortion( m_distortion * md, int dist){//name_dists *dist/*, unsigned char right*/){
	if (dist == -1){
		distorsion_channel = f_dist[md->_last_dist_active];
	}else {
		md->_d_left = dist;
		distorsion_channel = f_dist[md->_d_left];
		if (dist <= 6){
			md->_last_dist_active = dist;
		}
	}
}

/*
void vol_up_md (m_distortion *md, speaker sp){
	if (md->_master_ch == master_on){		//esto quizas es redundante ya q master es un bool
		volume_up(md->_d_ch);

		printf("setting volume for both channels\n");
	} else {
		if(sp){
			volume_up(md->__d_ch);
		} else {
			volume_up(md->_s_right);
		}
	}
}

void vol_down_md (m_distortion *md, speaker sp){
	if (md->_master_ch){		//x ser  redundante ya q master es un bool no agrego comparacion
		volume_down(md->_s_left);
		volume_down(md->_s_right);
		printf("setting volume for both channels\n");
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
		printf("setting gain for both channels\n");
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
		printf("setting gain for both channels\n");
	} else {
		if(sp){
			gain_down(md->_s_left);
		} else {
			gain_down(md->_s_right);
		}
	}
}

void reset_gain(m_distortion *md, speaker sp){
	if (md->_master_ch){
		md->_s_left->_dgain = 1.0;
		md->_s_right->_dgain = 1.0;
	} else {
		if (sp == speaker_izq){
			md->_s_left->_dgain = 1.0;
		} else {
			md->_s_right->_dgain = 1.0;		
		}
	}
}

void reset_vol(m_distortion *md, speaker sp){	
	if (md->_master_ch){
		md->_s_left->_dvol = 1.0;
		md->_s_right->_dvol = 1.0;
	} else {
		if (sp == speaker_izq){
			md->_s_left->_dvol = 1.0;
		} else {
			md->_s_right->_dvol = 1.0;		
		}
	}
}*/
