/*Simple 3 band equaliser with adjustable low and high frequencies ...
Fairly fast algo, good quality output (seems to be accoustically transparent with all gains set to 1.0)
How to use ...
1. First you need to declare a state for your eq
EQSTATE eq;
2. Now initialise the state (we'll assume your output frequency is 48Khz)

set_3band_state(eq,880,5000,480000);
Your EQ bands are now as follows (approximatley!)
low band = 0Hz to 880Hz
mid band = 880Hz to 5000Hz
high band = 5000Hz to 24000Hz

3. Set the gains to some values ...
eq.lg = 1.5; // Boost bass by 50%
eq.mg = 0.75; // Cut mid by 25%
eq.hg = 1.0; // Leave high band alone 

4. You can now EQ some samples
out_sample = do_3band(eq,in_sample)

*/
#ifndef __EQ3BAND__
#define __EQ3BAND__

typedef struct _EQSTATE {
  // Filter #1 (Low band)
  float  lf;       // Frequency
  float  f1p0;     // Poles ...
  float  f1p1;     
  float  f1p2;
  float  f1p3;

  // Filter #2 (High band)
  float  hf;       // Frequency
  float  f2p0;     // Poles ...
  float  f2p1;
  float  f2p2;
  float  f2p3;

  // Sample history buffer
  float  sdm1;     // Sample data minus 1
  float  sdm2;     //                   2
  float  sdm3;     //                   3

  // Gain Controls
  float  lg;       // low  gain
  float  mg;       // mid  gain
  float  hg;       // high gain
  
} EQSTATE;  

void  set_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq);
float do_3band(EQSTATE* es, float sample);

#endif
