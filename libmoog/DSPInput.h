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
 * $Revision: 1.2 $$Date: 2004/03/31 12:01:18 $
 */
#ifndef _DSPINPUT_H
#define _DSPINPUT_H

#include "MoogObject.h"

#define O_ADC_SIG(n) (n)

class DSPDevice;

class DSPInput: public MoogObject
{
	short *snddata;
	int bufsize;
	int offset;

	DSPDevice *dsp;
	int myDsp;

	void setup();

public:
	DSPInput(DSPDevice *dsp);
	DSPInput(char *dspdev, int _sampRate, int _channels);

	~DSPInput();

	void sampleGo();

	/* RTTI */
	const char *getClassName()
	{
		return "DSPInput";
	}
};

#endif /* _DSPINPUT_H */
