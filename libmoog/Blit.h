#pragma once


static double pi   =  3.1415926535897932384626433832795029L;
static double twopi = 6.2831853071795864769252867665590058L;

/* Leaky integrator/first order lowpass which 
 * shapes the impulse train into a nice
 * -6dB/octave spectrum
 *
 * The cutoff frequency needs to be pretty lowish
 * or the sawtooth will suffer phase distortion
 * at the low end.
 */

typedef struct
{
  double x1, y1;
  double a, b;
} lowpass_t;


/* dsf blit datatype
 *
 */

typedef struct
{
  double phase;    /* phase accumulator */
  double aNQ;      /* attenuation at nyquist */
  double curcps;   /* current frequency, updated once per cycle */
  double curper;   /* current period, updated once per cycle */
  lowpass_t leaky; /* leaky integrator */
  double N;        /* # partials */
  double a;        /* dsf parameter which controls roll-off */
  double aN;       /* former to the N */
} blit_t;
