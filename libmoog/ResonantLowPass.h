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
#ifndef _LIBMOOG_LOWPASS_H
#define _LIBMOOG_LOWPASS_H

#include "MoogObject.h"
#include <windows.h>

//#define MOOGVCF

#ifndef MOOGVCF
#define SECTIONS 2
struct BiQuad
{
	double a0, a1, a2;
	double b0, b1, b2;
};
#endif


class ResonantLowPass: public MoogObject
{
#ifndef MOOGVCF
	friend void ResonantLowPass_gainChanged(MoogObject *, double, long);
	friend void ResonantLowPass_cutoffChanged(MoogObject *, double, long);
	friend void ResonantLowPass_resonanceChanged(MoogObject *, double, long);

	double *hist;
	double *coef;

	



	//copied from filterprototype
	BiQuad *pcoef;
	void setSectionCoef(int, double, double, double, double, double, double);
#endif

	//FilterPrototype *proto;

#ifdef MOOGVCF
	double out1;
	double in1;
	double out2;
	double in2;
	double out3;
	double in3;
	double out4;
	double in4;
#endif


	double gain;
	double resonance;
	double cutoff;

	Output *output;
	double *inSig;
	double *inGain;
	double *inCutoff;
	double *inResonance;

	double fixedGain;
	void init();
#ifndef MOOGVCF


	void gainChanged(double);
	void cutoffChanged(double);
	void resonanceChanged(double);
#endif

public:

	ResonantLowPass(Scheduler *sched);
	~ResonantLowPass();

	void connectTo(ConnectionInfo *info);
	void disconnectTo(ConnectionInfo *info);
#ifndef MOOGVCF
	void recalcFilter();
#endif
	void sampleGo();

	void sync();

	const char *getClassName()
	{
		return "ResonantLowPass";
	}
};



#endif /* _LIBMOOG_LOWPASS_H */
