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
 * $Revision: 1.3 $$Date: 2004/09/04 10:39:53 $
 */

#include <libmoogutil/String.h>
#include "Control.h"

//types

void learn(MoogObject *obj, double data, long userdata)
{
    ((Control *)obj)->midilearn(data, userdata);
}
void controlmap(MoogObject *obj, double data, long userdata)
{
    ((Control *)obj)->controlMapping(data, userdata);
}
void Control::controlMapping(double data, long userdata)
{
    getOutput((String)"control" + userdata + "_change")->setData(data);
}
void Control::midilearn(double data, long userdata)
{
    if (data > 0)
        learning = true;
    else
        learning = false;
}

Control::Control(Scheduler *sched): MoogObject(sched, NULL)
{

    // for (int i=0;i<128;i++)
    {
//      addInput((String)"map_control_"+i,controlmap,i,0);
    }
//   addInput("learn"
    addOutput("midi_learn", "MIDI Control Learn", WAY2, false); // midi control learn function
    learning = false;
}
