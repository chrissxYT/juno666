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
 * $Revision: 1.10 $$Date: 2004/04/17 13:46:21 $
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#ifndef DEFAULT_SAMPLE_CONTROL_RATIO
#define DEFAULT_SAMPLE_CONTROL_RATIO 32
#endif

#include <libmoogutil/list.h>
#include <windows.h>
//#include <pthread.h>

class GoObject;
class DSPOutput;

DWORD WINAPI runSynth(void *);

#define CPS(x) (double)(x)/schedule->nyquistFreq

class Scheduler
{
    friend DWORD WINAPI runSynth(void *);

    list_head controlRateList;
    list_head sampleRateList;

    DSPOutput *dsp;
//#ifdef POSIX
//    static pthread_t tickThread;
//#else
    HANDLE tickThread;
//#endif

    void safeListOp(list_head *node, list_head *list, bool add);

#ifdef TARGET_VST
    list_head *currentListIter;
#endif

public:
    int nextGoHandle;
    void run();

    void Init();
    void DeInit();

    int sampleRate;
    int sampleControlRatio;
    double controlRate;
    double nyquistFreq;
    int suspended;

    void setSampleRate(int actual);
    void setSampleControlRatio(int);

    void scheduleControlRate(GoObject *, bool);
    void scheduleSampleRate(GoObject *, bool);

    void start(DSPOutput *);
    void start();
    void stop();

    void suspend();
    void resume();
};


#endif /* SCHEDULER_H */
