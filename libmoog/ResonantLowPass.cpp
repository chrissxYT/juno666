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
#include <math.h>
#include <libmoogutil/debug.h>
#include "ResonantLowPass.h"
#include "ConnectionInfo.h"
#include "Scheduler.h"
#ifndef MOOGVCF
#include "bilinear.h"





void ResonantLowPass::setSectionCoef(int section,
	double a0,
	double a1,
	double a2,
	double b0,
	double b1,
	double b2)
{
	if (section < 0 || section >= SECTIONS)
	{
		debug(DEBUG_APPERROR, "ResonantLowPass::setSectionCoef section of of range");
		return;
	}

	pcoef[section].a0 = a0;
	pcoef[section].a1 = a1;
	pcoef[section].a2 = a2;
	pcoef[section].b0 = b0;
	pcoef[section].b1 = b1;
	pcoef[section].b2 = b2;
}
#else
#include <libmoogutil/valuehelper.h>
#endif

/* ************************ */
#ifndef MOOGVCF
void ResonantLowPass_gainChanged(MoogObject *o, double data, long)
{
	((ResonantLowPass *)o)->gainChanged(data);
}

void ResonantLowPass_cutoffChanged(MoogObject *o, double data, long)
{
	((ResonantLowPass *)o)->cutoffChanged(data);
}

void ResonantLowPass_resonanceChanged(MoogObject *o, double data, long)
{
	((ResonantLowPass *)o)->resonanceChanged(data);
}
#endif
ResonantLowPass::ResonantLowPass(Scheduler *sched): MoogObject(sched, NULL)
{

#ifndef MOOGVCF
	pcoef = new BiQuad[SECTIONS];
	setSectionCoef(0, 1.0, 0, 0, 1.0, 0.765367, 1.0);
	setSectionCoef(1, 1.0, 0, 0, 1.0, 1.847759, 1.0);
#endif
	init();
}

void ResonantLowPass::init()
{

#ifndef MOOGVCF
	hist = new double[2 * SECTIONS];
	coef = new double[4 * SECTIONS+1];

	memset(hist, 0, sizeof(double) * 2 * SECTIONS);
	memset(coef, 0, sizeof(double) * 4 * SECTIONS);
#endif
#ifdef MOOGVCF
	addPorts("sig", INPUT, NULL,
		"gain", INPUT, NULL,
		"cutoff", INPUT, NULL,
		"resonance", INPUT, NULL,
		"sig", OUTPUT, true, NULL);
#else

	addPorts("sig", INPUT, NULL,
		"gain", INPUT, ResonantLowPass_gainChanged, 0, 1,
		"cutoff", INPUT, ResonantLowPass_cutoffChanged, 0, 1,
		"resonance", INPUT, ResonantLowPass_resonanceChanged, 0, 1,
		"sig", OUTPUT, true, NULL);
#endif
	output = &outputs[0];
	inSig = inputs[0].data;
	inGain = inputs[1].data;
	inCutoff = inputs[2].data;
	inResonance = inputs[3].data;

	schedule->scheduleSampleRate(this, true);
}

ResonantLowPass::~ResonantLowPass()
{
#ifndef MOOGVCF
	delete[]hist;
	delete[]coef;
#endif
}

void ResonantLowPass::connectTo(ConnectionInfo *info)
{
	MoogObject::connectTo(info);
	inSig = inputs[0].data;
	inGain = inputs[1].data;
	inCutoff = inputs[2].data;
	inResonance = inputs[3].data;
}

void ResonantLowPass::disconnectTo(ConnectionInfo *info)
{
	MoogObject::disconnectTo(info);
	inSig = inputs[0].data;
	inGain = inputs[1].data;
	inCutoff = inputs[2].data;
	inResonance = inputs[3].data;
}
#ifndef MOOGVCF
void ResonantLowPass::gainChanged(double data)
{
	if (data > 1)
		data = 1;
	if (data < 0)
		data = 0;
	gain = data;
	recalcFilter();

}

void ResonantLowPass::cutoffChanged(double data)
{
	if (data > 1)
		data = 1;
	if (data < 0)
		data = 0;
	if (data == 0)
		data = 0.001;
	if (data == 1)
		data = 0.999;
	cutoff = data;
	recalcFilter();
}

void ResonantLowPass::resonanceChanged(double data)
{
	if (data > 1)
		data = 1;
	if (data < 0)
		data = 0;
	if (data == 0)
		data = 0.001;
	if (data == 1)
		data = 0.999;
	resonance = data;
	recalcFilter();

}
#endif
#ifndef MOOGVCF
void ResonantLowPass::recalcFilter()
{

	double *coefptr = coef;
	fixedGain = gain;
	for (int i = 0;i < SECTIONS;i++)
	{
		szxform(pcoef[i].a0,
			pcoef[i].a1,
			pcoef[i].a2,
			pcoef[i].b0,
			// the resonance of the filter, the Q, is actually the Q
			// of each section of the filter multiplied together.
			// but the actual Q should go from 1 to 1000.  Since this
			// is a two section filter, and we use the same Q for each
			// section, the max Q should be sqrt(1000) or 30.6227
			// FIXME: resonance should probably not be linear
			pcoef[i].b1 / (sqrt(resonance) * 30.6227 + 1),
			pcoef[i].b2,
			cutoff * schedule->nyquistFreq,
			schedule->sampleRate,
			&fixedGain,
			coefptr);

		coefptr += 4;
	}
	//coef[0] = fixedGain;



}
#endif
#define denormal_fix  0.00000001

#ifndef MOOGVCF
void ResonantLowPass::sampleGo()
{
	double *coefptr = coef;
	double *hist1ptr, *hist2ptr;
	double newhist, hist1, hist2;
	double tmpOutput;

	hist1ptr = hist;
	hist2ptr = hist + 1;

	tmpOutput = *inSig * fixedGain;

	for (int i = 0;i < SECTIONS;i++)
	{
		hist1 = *hist1ptr;
		hist2 = *hist2ptr;

		tmpOutput = tmpOutput - hist1 * (*coefptr++);

		newhist = tmpOutput - hist2 * (*coefptr++);

		tmpOutput = newhist + hist1 * (*coefptr++);

		tmpOutput = tmpOutput + hist2 * (*coefptr++);

		*hist2ptr++ = *hist1ptr;
		*hist1ptr++ = newhist;
		hist1ptr++;
		hist2ptr++;
	}

	output->setData(tmpOutput);

}
#else
void ResonantLowPass::sampleGo()
{
	/*
	automatic slide adjustment. prevents strange filter effects
	*/
// Karlsen 24dB Filter by Ove Karlsen / Synergy-7 in the year 2003.
// b_f = frequency 0..1
// b_q = resonance 0..50
// b_in = input
// to do bandpass, subtract poles from eachother, highpass subtract with input.


		double b_in = *inSig; // before the while statement.
		double b_f = *inCutoff;
		double b_q = *inResonance * 30.6227 ;
		int b_oversample=0;
		double b_inSH = b_in;
    while (b_oversample < 2) {                        //2x oversampling (@44.1khz)
        double prevfp;
        prevfp = b_fp;
        if (prevfp > 1) {prevfp = 1;}                    // Q-limiter

        b_fp = (b_fp * 0.418) + ((b_q * pole4) * 0.582);        // dynamic feedback
        double intfp;
        intfp = (b_fp * 0.36) + (prevfp * 0.64);            // feedback phase
        b_in =    b_inSH - intfp;                        // inverted feedback        
                    
        pole1 = (b_in   * b_f) + (pole1 * (1 - b_f));            // pole 1
        if (pole1 > 1) {pole1 = 1;} else if (pole1 < -1) {pole1 = -1;}  // pole 1 clipping
        pole2 = (pole1   * b_f) + (pole2 * (1 - b_f));            // pole 2
        pole3 = (pole2   * b_f) + (pole3 * (1 - b_f));            // pole 3
        pole4 = (pole3   * b_f) + (pole4 * (1 - b_f));            // pole 4

        b_oversample++;
        }
	output->setData(pole4);

}

#endif

void ResonantLowPass::sync()
{
#ifndef MOOGVCF
	memset(hist, 0, sizeof(double) * 2 * SECTIONS);
	memset(coef, 0, sizeof(double) * 4 * SECTIONS);
#endif
}
