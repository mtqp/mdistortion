// ----------
//| Includes |
// ----------

#include <math.h>
#include <string.h>
#include "m_equalizer.h"

#ifndef bool
    #define bool unsigned char
    #define true 1
    #define false 0
#endif


// -----------
//| Constants |
// -----------

static double vsa = (1.0 / 4294967295.0);   // Very small amount (Denormal Fix)


// ---------------
//| Initialise EQ |
// ---------------

// Recommended frequencies are ...
//
//  lowfreq  = 880  Hz
//  highfreq = 5000 Hz
//
// Set mixfreq to whatever rate your system is using (eg 48Khz)

void init_equalizer(m_equalizer *es, int lowfreq, int highfreq, int mixfreq)
{
	// Clear state 

	memset(es,0,sizeof(m_equalizer));

	// Set Low/Mid/High gains to unity

	es->bass_eq = 1.0;
	es->mid_eq  = 1.0;
	es->treb_eq = 1.0;

	// Calculate filter cutoff frequencies

	es->bass_frecuency = 2 * sin(M_PI * ((float)lowfreq / (float)mixfreq)); 
	es->treb_frecuency = 2 * sin(M_PI * ((float)highfreq / (float)mixfreq));
}


// ---------------
//| EQ one sample |
// ---------------

// - sample can be any range you like :)
//
// Note that the output will depend on the gain settings for each band 
// (especially the bass) so may require clipping before output, but you 
// knew that anyway :)

float equalize(m_equalizer *es, float sample)
{
	float l,m,h;      // Low / Mid / High - Sample Values

	// Filter #1 (lowpass)

	es->f1p0  += (es->bass_frecuency * (sample   - es->f1p0)) + vsa;
	es->f1p1  += (es->bass_frecuency * (es->f1p0 - es->f1p1));
	es->f1p2  += (es->bass_frecuency * (es->f1p1 - es->f1p2));
	es->f1p3  += (es->bass_frecuency * (es->f1p2 - es->f1p3));

	l          = es->f1p3;

	// Filter #2 (highpass)

	es->f2p0  += (es->treb_frecuency * (sample   - es->f2p0)) + vsa;
	es->f2p1  += (es->treb_frecuency * (es->f2p0 - es->f2p1));
	es->f2p2  += (es->treb_frecuency * (es->f2p1 - es->f2p2));
	es->f2p3  += (es->treb_frecuency * (es->f2p2 - es->f2p3));

	h          = es->sdm3 - es->f2p3;

	// Calculate midrange (signal - (low + high))

	m          = es->sdm3 - (h + l);

	// Scale, Combine and store

	l         *= es->bass_eq;
	m         *= es->mid_eq;
	h         *= es->treb_eq;

	// Shuffle history buffer 

	es->sdm3   = es->sdm2;
	es->sdm2   = es->sdm1;
	es->sdm1   = sample;                

	// Return result

	return(l + m + h);
}

void set_treble(m_equalizer *eq, bool up){
	if (up)	eq->treb_eq += 0.2;
	else	eq->treb_eq -= 0.2; 
}

void set_bass(m_equalizer *eq, bool up){
	if (up)	eq->bass_eq += 0.2;
	else	eq->bass_eq -= 0.2;
}

void set_mid(m_equalizer *eq, bool up){
	if (up)	eq->mid_eq += 0.2;
	else	eq->mid_eq -= 0.2;
}

void reset_equalizer(m_equalizer *eq){
	eq->treb_eq = 1.0;
	eq->bass_eq = 1.0;
	eq->mid_eq  = 1.0;
}





