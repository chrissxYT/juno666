/*
 * Copyright(c) 2000 UltraMaster Group
 *
 * License to use UltraMaster Juno-6 is provided free of charge subject to the
 * following restrictions:
 *
 * 1.) This license is for your personal use only.
 *
 * 2.) No portion of this software may be redistributed by you to any other
 *     person in any form.
 *
 * 3.) You may not sell UltraMaster Juno-6 to any person.
 *
 * 4.) UltraMaster Juno-6 is provided without any express or implied warranty.
 *     In no event shall UltraMaster Group be held liable for any damages
 *     arising from the use of UltraMaster Juno-6.
 */
/**
 * Copyright (c) UltraMaster Group, LLC. All Rights Reserved.
 * $Revision: 1.11 $$Date: 2004/09/08 16:57:52 $
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "JunoSaw.h"


/* initializes a lowpass, sets cutoff/leakiness */

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







#include "Scheduler.h"


#define FRQ   0
#define AMP   1

void JunoSaw_frqChanged(MoogObject *o, double, long)
{
	((JunoSaw *)o)->frqChanged();
}

void JunoSaw_sync(MoogObject *o, double, long)
{
	((JunoSaw *)o)->sync();
}

JunoSaw::JunoSaw(Scheduler *sched): MoogObject(sched, NULL)
{
	addPorts("frq", INPUT, JunoSaw_frqChanged, 0, schedule->sampleControlRatio,
		"amp", INPUT, NULL,
		"sync", INPUT, JunoSaw_sync, 0, 1,
		"sig", OUTPUT, true,
		"sync", OUTPUT, false,
		NULL);
	init_blit(&b, 0.5, 0.0001);

	output = &outputs[0];
	inFrq = inputs[FRQ].data;
	inAmp = inputs[AMP].data;
	inSync = inputs[2].data;

	frq = 0;
	pos = 1.0;
	lastTrigger = 0;
}

void JunoSaw::connectTo(ConnectionInfo *info)
{
	MoogObject::connectTo(info);
	inFrq = inputs[FRQ].data;
	inAmp = inputs[AMP].data;
	inSync = inputs[2].data;
}

void JunoSaw::disconnectTo(ConnectionInfo *info)
{
	MoogObject::disconnectTo(info);
	inFrq = inputs[0].data;
	inAmp = inputs[1].data;
	inSync = inputs[2].data;
}
#define ASIMUL 128
void JunoSaw::frqChanged()
{
	newFrq = *inFrq / 2;


	//newFrq = *inFrq / 2;

	//MOOG_DEBUG("newFrq=%f", newFrq);

	if (newFrq < 0.0)
		newFrq *= -1.0;

	if (newFrq == 0.0)
	{
		schedule->scheduleSampleRate(this, false);
		newFrq = 0.0;
	}
	else if (!isSampleScheduled())
		schedule->scheduleSampleRate(this, true);
	//newfrq = abs((double)((newfrq - newFrq) / (double)ASIMUL));

}

void JunoSaw::sync()
{
	pos = 1.0;
}

void JunoSaw::sampleGo()
{

	double result = update_blit(&b,newFrq);
	output->setData(result* (*inAmp));

	/*
	pos += frq;

	if (pos >= 1.0)
	{
		lastTrigger = 1;
		outputs[1].setData(1);

		pos--;

		frq = newFrq;
		output->data = -*inAmp;
		power = *inAmp * (frq * 2.0 + .0072);
		dampening = -*inAmp * (2.1 * frq * frq + .01 * frq + .000022);
	}

	else if (lastTrigger == 1)
	{
		lastTrigger = 0;
		outputs[1].setData(0);
	}

	output->setData(output->data + power);
	power += dampening;

	if (power < 0)
	{
		power = -.0004 * *inAmp;
		dampening = 0;
	}
*/
}
