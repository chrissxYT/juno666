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
 * $Revision: 1.12 $$Date: 2004/09/10 10:56:58 $
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "JunoSaw.h"






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

	if (*inAmp==0.0)
	{
		output->setData(0.0);
		return;
	}
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
