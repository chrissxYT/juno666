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

#include "VSTOutput.h"
#include "libmoog/Scheduler.h"
#include "libmoog/JunoControl.h"

void setPanningValues(MoogObject *o, double data, long)
{
    ((VSTOutput *)o)->setPanning(data);
}

VSTOutput::VSTOutput(JunoControl *jc)
{
    addInput("panning", setPanningValues, 0, 1);
    setup();
    PATCH(jc, "panning", this, "panning");
    setPanningValues(this, 0.5, 0);
}

void VSTOutput::setup()
{
    char tmpname[16];

    inSig = new double *[2];
    inAmp = new double *[2];

    for (int i = 0;i < 2;i++)
    {
        sprintf(tmpname, "sig%d", i);
        inSig[i] = addInput(tmpname)->data;
        sprintf(tmpname, "amp%d", i);
        inAmp[i] = addInput(tmpname)->data;
    }

    dataWrittenCallback = NULL;

    Scheduler::setSampleRate(SAMPLE_RATE_44k);

    Scheduler::scheduleSampleRate(this, true);
}

VSTOutput::~VSTOutput()
{
    delete[]inSig;
    delete[]inAmp;
}

void VSTOutput::connectTo(ConnectionInfo *info)
{
    MoogObject::connectTo(info);

    for (int i = 0;i < 2;i++)
    {
        inSig[i] = inputs[2 * i+1].data;
        inAmp[i] = inputs[2 * i+2].data;
    }
}

void VSTOutput::disconnectTo(ConnectionInfo *info)
{
    MoogObject::disconnectTo(info);

    for (int i = 0;i < 2;i++)
    {
        inSig[i] = inputs[2 * i+1].data;
        inAmp[i] = inputs[2 * i+2].data;
    }
}

void VSTOutput::setPanning(double data)
{
    if (data < 0)
        return;
    if (data > 1)
        return;
    panright = sqrt(data);
    panleft = sqrt(1 - data);
}

void VSTOutput::sampleGo()
{
    (*out1++) = (float)(*inSig[0] * *inAmp[0] * panright);
    (*out2++) = (float)(*inSig[1] * *inAmp[1] * panleft);
}
