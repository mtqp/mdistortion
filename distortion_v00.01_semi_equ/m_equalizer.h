#ifndef __M_EQUALIZER_H__
#define __M_EQUALIZER_H__

#ifndef bool
    #define bool unsigned char
    #define true 1
    #define false 0
#endif

typedef struct _m_equalizer {
  // Filter #1 (Low band)

  double  bass_frecuency;       // Frequency
  double  f1p0;     // Poles ...
  double  f1p1;     
  double  f1p2;
  double  f1p3;

  // Filter #2 (High band)

  double  treb_frecuency;       // Frequency
  double  f2p0;     // Poles ...
  double  f2p1;
  double  f2p2;
  double  f2p3;

  // Sample history buffer

  double  sdm1;     // Sample data minus 1
  double  sdm2;     //                   2
  double  sdm3;     //                   3

  // Gain Controls

  double  bass_eq;       // low  gain
  double  mid_eq;       // mid  gain
  double  treb_eq;       // high gain

} m_equalizer;

void   init_equalizer(m_equalizer* es, int lowfreq, int highfreq, int mixfreq);
double equalize(m_equalizer* es, double sample);
void set_treble(m_equalizer *eq, bool up_down);
void set_bass(m_equalizer *eq, bool up_down);
void set_mid(m_equalizer *eq, bool up_down);
void reset_equalizer(m_equalizer *eq);

#endif

/*
Simple 3 band equaliser with adjustable low and high frequencies ...

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


Have fun and mail me if any problems ... etanza at lycos dot co dot uk

*/


//----------------------------------------------------------------------------




