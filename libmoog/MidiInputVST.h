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
 * $Revision: 1.2 $$Date: 2004/09/11 00:12:34 $
 */
#ifndef MIDIINPUTVST_H
#define MIDIINPUTVST_H

#include "MoogObject.h"
#include "Control.h"


/* Note, there should be various modes ( there are more than these )
1. monophonic, after key pressed other keys are ignored
2. monophonic, keys are pushed onto a stack(list),
     pressing key replaces, releasing key if current reverts to most recent
     key on list, otherwise releasing removes key from list.
3. polyphonic, after n keys, keys are ignored ( this handles case 1 )
4. polyphonic, after n keys last key pressed replaces first key pressed
also to think about portamento, glissando, unison, pitch bend, expression
*/

#define O_MIDI_CTL(x) (x)
#define O_MIDI_BND    127
#define O_MIDI_SIG(x) (2*(x) + 128)
#define O_MIDI_AMP(x) (2*(x) + 129)

struct midi_voice
{
    int note;
    Output *pitchOutput;
    Output *gateOutput;
};

class MidiInput: public MoogObject
{
//    friend void *midi_input_run(void *);

public:
    int running;
    int nvoices;
    bool isUniSono;
    int lastNote;
    int holdPressed;
    struct midi_voice *voices;
    int *savedGateInfo;

    Control *control;

    inline void *run();
    inline void allNotesOff();
    void doNoteOn(unsigned int c, unsigned int n, unsigned int v);
    void doNoteOff(unsigned int c, unsigned int n, unsigned int v);
    inline void doPitchBend(unsigned int amount);
    inline void holdChanged(double data);
    inline void doUniSono(double data);

    MidiInput(Control *jc, int nv, Scheduler *sched);
    ~MidiInput();

    void start();
    void stop();

    void proc(unsigned char cmd, unsigned char channel, unsigned char *data);

    inline virtual const char *getClassName()
    {
        return "MidiInput";
    }

    inline Output * getOutput(const char *n);
};

#endif /* MIDIINPUTVST_H */
