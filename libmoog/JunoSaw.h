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
 * $Revision: 1.5 $$Date: 2004/06/09 15:35:34 $
 */
#ifndef JUNOSAW_H
#define JUNOSAW_H

#include "MoogObject.h"

class JunoSaw: public MoogObject
{
public:
	double *inAmp;
private:
	friend void JunoSaw_frqChanged(MoogObject *, double, long);
	friend void JunoSaw_sync(MoogObject *, double, long);

	void frqChanged();

	Output *output;
	double *inFrq;
	
	double *inSync;

	double pos;
	double frq;
	double newfrq;
	double destfrq;

	double newFrq;
	double power;
	double dampening;
	double sign;

	int lastTrigger;

public:
	JunoSaw(Scheduler *sched);

	void connectTo(ConnectionInfo *info);
	void disconnectTo(ConnectionInfo *info);

	void sync();
	void sampleGo();
	const char *getClassName()
	{
		return ("JunoSaw");
	}
};

#endif /* JUNOSAW_H */
