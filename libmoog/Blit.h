#pragma once

#include <math.h>


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
  double curpos;
  double curcps;   /* current frequency, updated once per cycle */
  double curper;   /* current period, updated once per cycle */
  lowpass_t leaky; /* leaky integrator */
  double N;        /* # partials */
  double a;        /* dsf parameter which controls roll-off */
  double aN;       /* former to the N */
} blit_t;


inline void init_lowpass(lowpass_t *lp, double cutoff)
{
  double Omega;

  lp->x1 = lp->y1 = 0.0;

  Omega = atan(pi * cutoff);
  lp->a = -(1.0 - Omega) / (1.0 + Omega);
  lp->b = (1.0 - lp->b) / 2.0;
}

inline double update_lowpass(lowpass_t *lp, double x)
{
  double y;

  y = lp->b * (x + lp->x1) - lp->a * lp->y1;

  lp->x1 = x;
  lp->y1 = y;

  return y;
}

/* initializes a blit structure
 *
 * The aNQ parameter is the desired attenuation
 * at nyquist. A low value yields a duller
 * sawtooth but gets rid of those annoying CLICKS
 * when sweeping the frequency up real high. |aNQ|
 * must be strictly less than 1.0! Find a setting
 * which works for you.
 *
 * The cutoff parameter controls the leakiness of
 * the integrator.
 */

inline void init_blit(blit_t *b, double aNQ, double cutoff)
{
  b->phase  = 0.0;
  b->aNQ = aNQ;
  b->curcps = 0.0;
  b->curpos = 0.0;
  b->curper = 0.0;
  init_lowpass(&b->leaky, cutoff);
}

/* Returns a sawtooth computed from a leaky integration
 * of a DSF bandlimited impulse train.
 *
 * cps (cycles per sample) is the fundamental
 * frequency: 0 -> 0.5 == 0 -> nyquist
 */

inline double update_blit(blit_t *b, double cps)
{
  double P2, beta, Nbeta, cosbeta, n, d, blit, saw;
  
  if(b->phase >= 1.0 || b->curcps == 0.0)
    {
      /* New cycle, update frequency and everything
       * that depends on it
       */

      if(b->phase >= 1.0)
b->phase -= 1.0;

      b->curcps = cps;        /* this cycle\'s frequency */
      b->curper = 1.0 / cps;  /* this cycle\'s period */

      P2 = b->curper / 2.0;
      b->N = 1.0 + floor(P2); /* # of partials incl. dc */

      /* find the roll-off parameter which gives
       * the desired attenuation at nyquist
       */

      b->a   = pow(b->aNQ, 1.0 / P2);
      b->aN  = pow(b->a,   b->N);
    }

  beta = twopi * b->phase;

  Nbeta = b->N * beta;
  cosbeta = cos(beta);

  /* The dsf blit is scaled by 1 / period to give approximately the same
   * peak-to-peak over a wide range of frequencies.
   */

  n = 1.0 -
    b->aN * cos(Nbeta) -
    b->a * (cosbeta - b->aN * cos(Nbeta - beta));
  d = b->curper * (1.0 + b->a * (-2.0 * cosbeta + b->a));

  b->phase += b->curcps; /* update phase */

  blit = n / d - b->curcps; /* This division can only fail if |a| == 1.0
     * Subtracting the fundamental frq rids of DC
     */

  saw = update_lowpass(&b->leaky, blit); /* shape blit spectrum into a saw */

  return saw;
}





