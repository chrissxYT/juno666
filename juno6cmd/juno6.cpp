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
 * $Revision: 1.4 $$Date: 2004/04/07 11:26:31 $
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <libmoog/moog.h>
#include <libmoogutil/String.h>
#include <sys/types.h>
#include <libmoogutil/debug.h>
#include <libmoog/JunoPatch.h>
#include <libmoog/JunoSynth.h>
#include "juno_keyboard.h"

String patchFileName;
juno_patch *patches;
MidiInput *midiInput = NULL;
int threadcount=0;
#ifdef DOMAIN
int main(int argc, char **argv)
#else
int Juno666(int argc, char **argv)
#endif
{
	debuglvl = DEBUG_SYSERROR | DEBUG_APPERROR | DEBUG_APPMSG1;
	
	if (argc > 1)
	{
		patchFileName = argv[1];
	}
	else
	{
		//FIXME: maybe this should come from settings???

		patchFileName = "juno6.patches";
	}

	

	Settings settings;
	puts("get settings");
	const char *numVoicesStr = settings.getString("juno6", "num-voices");
	int numVoices = 6;

	if (strcmp(numVoicesStr, "") != 0)
		numVoices = atoi(numVoicesStr);
	Scheduler::Init();
	JunoControl *junoControl = new JunoControl(numVoices);
	if (settings.getInt("devices", "use-midi"))
	{
		midiInput = new MidiInput(junoControl,settings.getString("devices",
			"midi-input"),
			numVoices);

		if (midiInput->isOpen())
		{
			midiInput->start();
		}
		else
		{
			delete midiInput;
			midiInput = NULL;
		}
	}
	
	initSynth(junoControl, &settings, midiInput, numVoices);


	patches = juno_patchset_new();
	load_patches(patchFileName, patches);
	JunoKeyboard *keyboard = new JunoKeyboard(numVoices);
	junoControl->MoogObject::getOutput("patch_change")->setData(0);
	puts("press any key to abort");
	getch();
	Scheduler::DeInit();
	exit(1);
	return (0);
}

#ifndef DOMAIN
extern "C" int Juno666main(int argc, char *argv[])
{
	return Juno666(argc, argv);
}
#endif
