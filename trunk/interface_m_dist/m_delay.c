
#include "m_delay.h"

m_delay* delay_new (int size, int total){
	m_delay* d = (m_delay*) malloc(sizeof(m_delay));
	if(d == NULL){
		printf("Couldn't malloc Delay Structure\n");
		return NULL;
	}
	else {
		d->dl_size = size;
		d->dl_total_bufs = total;
		d->dl_cant_bufs_active = 4;
		d->dl_sub_i= 0;
		d->dl_speed = size/2;	//setear la speed en la inicial de glade

		d->dl_bufs = (float**) malloc(d->dl_total_bufs*sizeof(float*));
		if(d->dl_bufs == NULL){
			printf("Couldn't malloc Buffer Delay Structure\n");
			return NULL;
		} else {
			int i;
			for(i=0;i<d->dl_total_bufs;i++){
				d->dl_bufs[i] = (float*) malloc(d->dl_size*sizeof(float));
			}

			int alguno_null = 0;	//ESTO ES FALSE!
			for(i=0;i<d->dl_total_bufs;i++){
				alguno_null |= (d->dl_bufs[i] == NULL);
			}

			if(alguno_null) {
				printf("Couldn't malloc Buffer Delay Structure\n");
				return NULL;
			} else {
				for(i=0;i<d->dl_total_bufs;i++){
					int j;
					for(j=0;j<d->dl_size;j++){ 
						d->dl_bufs[i][j] = 0.0;
					}
				}
				printf("	Delay Effect Set\n");
				return d;
			}
		}
	}
}
