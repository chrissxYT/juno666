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
 * $Revision: 1.8 $$Date: 2004/06/25 10:38:42 $
 */
#ifndef ADSR_H
#define ADSR_H

#include "MoogObject.h"

#define ATTACK	 0
#define DECAY	 1
#define SUSTAIN  2
#define RELEASE  3
#define FINISHED 4

class ADSR: public MoogObject
{
private:
	friend void adsr_attack_changed(MoogObject *, double, long);
	friend void adsr_decay_changed(MoogObject *, double, long);
	friend void adsr_sustain_changed(MoogObject *, double, long);
	friend void adsr_release_changed(MoogObject *, double, long);
	friend void adsr_amp_changed(MoogObject *, double, long);
	friend void adsr_trigger_changed(MoogObject *, double, long);

	void attackChanged(double);
	void decayChanged(double);
	void sustainChanged(double);
	void releaseChanged(double);
	void ampChanged(double);
	void triggerChanged(double);

	int nosound;

protected:
	int state;

	double attack;
	double decay;
	double sustain;
	double release;
	double amp;

	Output *output;

public:
	ADSR(Scheduler *scheduler, double a = 0, double d = 0, double s = 1,
		double r = 0, double amp = 1);

	int isPlaying()
	{
		if (state == SUSTAIN)
		{
			if (output->data <= 0.0)
				return false;
		}
		return (state != FINISHED);
	}

	void sampleGo();

	const char *getClassName()
	{
		return "ADSR";
	}
};

#endif /* ADSR_H */
