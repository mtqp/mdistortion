/* Simple implementation of Biquad filters -- Tom St Denis
*
* Based on the work

Cookbook formulae for audio EQ biquad filter coefficients
---------------------------------------------------------
by Robert Bristow-Johnson, pbjrbj@viconet.com  a.k.a. robert@audioheads.com

* Available on the web at

http://www.smartelectronix.com/musicdsp/text/filters005.txt

* Enjoy.
*
* This work is hereby placed in the public domain for all purposes, whether
* commercial, free [as in speech] or educational, etc.  Use the code and please
* give me credit if you wish.
*
* Tom St Denis -- http://tomstdenis.home.dhs.org*/

#ifndef __M_EQ_H__
#define __M_EQ_H__

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef M_LN2
#define M_LN2	0.69314718055994530942
#endif

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

typedef struct {
	float a0, a1, a2, a3, a4;
	float x1, x2, y1, y2;
	float _dbgain, _freq, _srate, _bandwidth;
} m_equalizer;

float equalize_sample(float sample, m_equalizer * b);
m_equalizer *band_EQ_new(int type, float dbGain, /* gain of filter */
                         float freq,             /* center frequency */
                         float srate,            /* sampling rate */
                         float bandwidth);       /* bandwidth in octaves */
void reset_eq_params(m_equalizer *bq, float bandwidth, int filter_type);

/* filter types */
enum {
   LPF, 	/* Low pass filter */
   HPF, 	/* High pass filter */
   BPF, 	/* Band pass filter */
   NOTCH, 	/* Notch Filter */
   PEQ, 	/* Peaking band EQ filter */
   LSH, 	/* Low shelf filter */
   HSH 		/* High shelf filter */
};

#endif
