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
#include <libmoogutil/sio.h>

#include "DSPOutput.h"

/* $Id: DSPOutput.cpp,v 1.5 2004/04/17 13:46:21 strepto Exp $ */

void setPanningValues(MoogObject *o, double data, long)
{
    ((DSPOutput *)o)->setPanning(data);
}


DSPOutput::DSPOutput(JunoControl *jc, Scheduler *sched, ConnectionManager *conn, DSPDevice *_dsp):
MoogObject(sched, conn),
dsp(_dsp)
{
    myDsp = 0;
    addInput("panning", setPanningValues, 0, 1);
    setup();
    PATCH(jc, "panning", this, "panning");
    setPanningValues(this, 0.5, 0);
}


DSPOutput::DSPOutput(JunoControl *jc, Scheduler *sched, ConnectionManager *conn,
    const char *device, int rate, int channels, int numFrags, int fragSize):
MoogObject(sched, conn),
scheduler(sched)
{
    dsp = new DSPDevice(device, DSP_WRITE, rate, channels, numFrags, fragSize);
    myDsp = 1;
    addInput("panning", setPanningValues, 0, 1);
    setup();
    PATCH(jc, "panning", this, "panning");
    setPanningValues(this, 0.5, 0);
}

void DSPOutput::setup()
{
    char tmpname[16];

    inSig = new double *[dsp->channels];
    inAmp = new double *[dsp->channels];

    for (int i = 0;i < dsp->channels;i++)
    {
        sprintf(tmpname, "sig%d", i);
        inSig[i] = addInput(tmpname)->data;
        sprintf(tmpname, "amp%d", i);
        inAmp[i] = addInput(tmpname)->data;
    }

    dataWrittenCallback = NULL;

    scheduler->setSampleRate(dsp->getSampleRate());

    if (dsp->isOpen())
        scheduler->scheduleSampleRate(this, true);
}

DSPOutput::~DSPOutput()
{
    if (myDsp)
        delete dsp;

    delete[]inSig;
    delete[]inAmp;
}

void DSPOutput::connectTo(ConnectionInfo *info)
{
    MoogObject::connectTo(info);

    for (int i = 0;i < dsp->channels;i++)
    {
        inSig[i] = inputs[2 * i+1].data;
        inAmp[i] = inputs[2 * i+2].data;
    }
}

void DSPOutput::disconnectTo(ConnectionInfo *info)
{
    MoogObject::disconnectTo(info);

    for (int i = 0;i < dsp->channels;i++)
    {
        inSig[i] = inputs[2 * i+1].data;
        inAmp[i] = inputs[2 * i+2].data;
    }
}


void DSPOutput::setPanning(double data)
{
    if (data < 0)
        return;
    if (data > 1)
        return;
    panright = sqrt(data);
    panleft = sqrt(1 - data);
}

void DSPOutput::sampleGo()
{
    int moved;
    if (dsp->channels == 1)
    {
        *dsp->writeDataPtr++ = (int)((*inSig[0] * *inAmp[0]) * 32768);
    }
    else
    {
        *dsp->writeDataPtr++ = (int)((*inSig[0] * *inAmp[0] * panright) * 32768);
        *dsp->writeDataPtr++ = (int)((*inSig[1] * *inAmp[1] * panleft) * 32768);
    }

    moved = dsp->checkFlushBuffers();

    if ((moved & DSP_WRITE) && dataWrittenCallback)
        dataWrittenCallback();
}

void DSPOutput::setDataWrittenCallback(void (*dwcb)(void))
{
    dataWrittenCallback = dwcb;
}
