
#include "m_delay.h"

m_delay* delay_new (int size){
	m_delay* d = (m_delay*) malloc(sizeof(m_delay));
	if(d == NULL){
		printf("Couldn't malloc Delay Structure\n");
		return NULL;
	}
	else {
		d->dl_size = size;
		d->dl_sub_i= 0;
		d->dl_buf1 = (float*) malloc(d->dl_size*sizeof(float));
		d->dl_buf2 = (float*) malloc(d->dl_size*sizeof(float));
		d->dl_buf3 = (float*) malloc(d->dl_size*sizeof(float));
		
		if((d->dl_buf1 == NULL) || (d->dl_buf2 == NULL) || (d->dl_buf2 == NULL)) {
			printf("Couldn't malloc Buffer Delay Structure\n");
			return NULL;
		} else {
			printf("	Delay Effect Set\n");
			int i;
			for(i=0;i<d->dl_size;i++){
				d->dl_buf1[i] = 0.0;
				d->dl_buf2[i] = 0.0;
				d->dl_buf3[i] = 0.0;
			}
			return d;
		}
	}
}
