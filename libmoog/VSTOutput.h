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
 * $Revision: 1.3 $$Date: 2004/04/16 14:39:00 $
 */
#ifndef _VSTOutput_H
#define _VSTOutput_H

#include "libmoog/MoogObject.h"

#define I_DSP_SIG(n) ((n) * 2)
#define I_DSP_AMP(n) ((n) * 2 + 1)

#define SAMPLE_RATE_44k 44100
#define SAMPLE_RATE_22k 22050
#define SAMPLE_RATE_8k	 8000

#include "libmoog/JunoControl.h"
#include "libmoog/ConnectionManager.h"

class VSTOutput: public MoogObject
{
	double **inSig;
	double **inAmp;
	double panleft;
	double panright;
	int channels;

	void (*dataWrittenCallback)(void);
	void setup();

	float *out1;
	float *out2;

public:
	VSTOutput(JunoControl *jc, Scheduler *sched);
	~VSTOutput();

	void connectTo(ConnectionInfo *info);
	void disconnectTo(ConnectionInfo *info);
	void setPanning(double data);
	void setStereo(double data);

//    void setDataWrittenCallback(void (*)(void));
	void sampleGo();

	void setOutput1(float *o1)
	{
		out1 = o1;
	}
	void setOutput2(float *o2)
	{
		out2 = o2;
	}

	/* RTTI */
	const char *getClassName()
	{
		return "VSTOutput";
	}
};

#endif /* _VSTOutput_H */
