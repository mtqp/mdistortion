#ifndef __DUMMY_AUDIO_H__
#define __DUMMY_AUDIO_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef float  dummy_audio_size;
typedef float* dummy_audio_buf;

typedef struct _dummy_audio{
	dummy_audio_size dummy_size;
	dummy_audio_buf  dummy_buf;
}dummy_audio;

dummy_audio* init_dummy_audio(dummy_audio_size dummy_size);
void reset_buf(dummy_audio* d);

#endif
