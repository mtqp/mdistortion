
#include "dummy_audio.h"

dummy_audio* init_dummy_audio(dummy_audio_size size){
	dummy_audio* d = (dummy_audio*) malloc(sizeof(dummy_audio));
	d->dummy_size = size;
	d->dummy_buf  = (dummy_audio_buf) malloc(sizeof(d->dummy_size*sizeof(dummy_audio_size)));
	int i;
	for(i=0;i<d->dummy_size;i++)
		d->dummy_buf[i] = sin(i);
	return d;
}

void reset_buf(dummy_audio* d){
	int i;
	for(i=0;i<d->dummy_size;i++)
		d->dummy_buf[i] = sin(i);	
}
