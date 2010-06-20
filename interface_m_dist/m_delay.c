
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
		d->dl_speed = size/2;	//puedo crecer 32 veces para arriva y 32 para abajo creo...
//		printf("LLEGAMOS A UNO\n");	

		d->dl_bufs = (float**) malloc(d->dl_total_bufs*sizeof(float*));
		if(d->dl_bufs == NULL){
			printf("Couldn't malloc Buffer Delay Structure\n");
			return NULL;
		} else {
//		printf("LLEGAMOS A dos\n");	
			int i;
			for(i=0;i<d->dl_total_bufs;i++){
				d->dl_bufs[i] = (float*) malloc(d->dl_size*sizeof(float));
			}
//		printf("LLEGAMOS A tres\n");	
			int alguno_null = 0;	//ESTO ES FALSE!
			for(i=0;i<d->dl_total_bufs;i++){
				alguno_null |= (d->dl_bufs[i] == NULL);
			}
//		printf("LLEGAMOS A 4\n");	
			if(alguno_null) {
				printf("Couldn't malloc Buffer Delay Structure\n");
				return NULL;
			} else {
				for(i=0;i<d->dl_total_bufs;i++){
					printf("i==%d\n",i);
					int j;
					for(j=0;j<d->dl_size;j++){ 
//					printf("j=%d\n",j);
						d->dl_bufs[i][j] = 0.0;
					}
				}
				printf("	Delay Effect Set\n");
				return d;
			}
		}
	}
}
