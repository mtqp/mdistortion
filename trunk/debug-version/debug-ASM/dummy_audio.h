#ifndef __DUMMY_AUDIO_H__
#define __DUMMY_AUDIO_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define dummy_audio_size int
#define dummy_audio_buf  float* 

typedef struct _dummy_audio{
	int 	dummy_size;
	float*  dummy_buf;
}dummy_audio;

dummy_audio* init_dummy_audio(int dummy_size);
void free_dummy_audio(dummy_audio* d);
void reset_buf(dummy_audio* d);

#endif
