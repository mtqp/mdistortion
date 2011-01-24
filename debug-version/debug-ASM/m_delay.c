
#include "m_delay.h"

/*
	Inicializa estructura DELAY
*/

m_delay* delay_new (int size, int total){
	m_delay* d = (m_delay*) malloc(sizeof(m_delay));
	if(d == NULL){
		printf("Couldn't malloc Delay Structure\n");
		return NULL;
	}
	else {
		d->dl_size = size;			//largo de los buffers (tiempo máximo de delay)
		d->dl_total_bufs = total;	//cantidad total de buffers disponibles
		d->dl_cant_bufs_active = 4; //cantidad de buffers activos inicial
		d->dl_sub_i= 0;				//posición inicial en los buffers
		d->dl_speed = 32768; 		//velocidad inicial de delay
		//size/2;	//setear la speed en la inicial de glade

		d->dl_bufs = (float**) malloc(d->dl_total_bufs*sizeof(float*));
		if(d->dl_bufs == NULL){
			printf("Couldn't malloc Buffer Delay Structure\n");
			return NULL;
		} else {
			int i;
			for(i=0;i<d->dl_total_bufs;i++){	//malloc de buffers
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
				for(i=0;i<d->dl_total_bufs;i++){	//limpia los buffers
					int j;
					for(j=0;j<d->dl_size;j++){ 
						d->dl_bufs[i][j] = 0.0;
					}
				}
				printf("	Delay Effect Set\n");
				printf("delay* = %d\n",(int) d);
				return d;
			}
		}
	}
}
