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
 * $Revision: 1.5 $$Date: 2004/04/14 12:28:55 $
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#ifndef DEFAULT_SAMPLE_CONTROL_RATIO
#define DEFAULT_SAMPLE_CONTROL_RATIO 32
#endif

#include <windows.h>
#include <libmoogutil/SimpleArray.h>
#include "GoObject.h"

class DSPOutput;

DWORD WINAPI runSynth(void *);

#define CPS(x) (double)(x)/Scheduler::nyquistFreq

typedef SimpleArray <GoObject> GoObjectArray; 



class Scheduler
{
    friend DWORD WINAPI runSynth(void *);
    static DSPOutput *dsp;
    static HANDLE tickThread;

public:
    static void run();

    static void Init();
    static void DeInit();

    static int sampleRate;
    static int sampleControlRatio;
    static double controlRate;
    static double nyquistFreq;
    static int suspended;

    static void setSampleRate(int actual);
    static void setSampleControlRatio(int);

    static void scheduleControlRate(GoObject *, bool);
    static void scheduleSampleRate(GoObject *, bool);

    static void start(DSPOutput *);
    static void start();
    static void stop();

    static void suspend();
    static void resume();
};


#endif /* SCHEDULER_H */
