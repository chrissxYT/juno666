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
#ifdef MOOGVCF
	in1 = 0.0;
	in2 = 0.0;
	in3 = 0.0;
	in4 = 0.0;
	out1 = 0.0;
	out2 = 0.0;
	out3 = 0.0;
	out4 = 0.0;
	resonance = 0.0;
	cutoff = 0.0;
	gain = 0.0;
#endif
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
	processParameter(&resonance,*inResonance,0.001);
	processParameter(&gain,*inGain,0.001);
	processParameter(&cutoff,*inCutoff,0.001);



	double i = *inSig * gain;
	double f = cutoff * 1.16;
	double fb = resonance * (30.6227 / 2) * (1.0 - 0.15 * f * f);
	i -= out4 * fb;
	i *= 0.35013 * (f * f) * (f * f);
	out1 = i + 0.3 * in1 + (1 - f) * out1; // Pole 1
	in1 = i;
	out2 = out1 + 0.3 * in2 + (1 - f) * out2; // Pole 2
	in2 = out1;
	out3 = out2 + 0.3 * in3 + (1 - f) * out3; // Pole 3
	in3 = out2;
	out4 = out3 + 0.3 * in4 + (1 - f) * out4; // Pole 4
	in4 = out3;
	output->setData(out4);
}

#endif

void ResonantLowPass::sync()
{
#ifndef MOOGVCF
	memset(hist, 0, sizeof(double) * 2 * SECTIONS);
	memset(coef, 0, sizeof(double) * 4 * SECTIONS);
#endif
}
