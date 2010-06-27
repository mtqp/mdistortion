
#include "dummy_audio.h"

dummy_audio* init_dummy_audio(int size){
	dummy_audio* d = (dummy_audio*) malloc(sizeof(dummy_audio));
	if(d==NULL){
		printf("Could't malloc DUMMY_AUDIO structure\n");
		return NULL;
	}else{
		d->dummy_size = size;
		d->dummy_buf  = (float*) malloc(d->dummy_size*sizeof(float));
		if(d->dummy_buf==NULL){
			printf("Could't malloc DUMMY_AUDIO structure\n");
			return NULL;
		}else{
			int i;
			for(i=0;i<d->dummy_size;i++){
				d->dummy_buf[i] = sin(i);
			}
			return d;
		}
	}
}

void free_dummy_audio(dummy_audio* d){
	free(d->dummy_buf);
	free(d);
}
void reset_buf(dummy_audio* d){
	int i;
	for(i=0;i<d->dummy_size;i++)
		d->dummy_buf[i] = sin(i);	
}
