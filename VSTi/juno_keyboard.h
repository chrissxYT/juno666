#ifndef _JUNO_KEYBOARD_H
#define _JUNO_KEYBOARD_H

#include <libmoog/Scheduler.h>

#include <libmoog/Output.h>
#include <libmoog/Control.h>
#include <libmoog/JunoPatch.h>
#include <libmoog/JunoControl.h>
#include <libmoog/MidiInputVST.h>
#include <libmoog/ConnectionManager.h>

class JunoKeyboard: public MoogObject
{
    friend void __fastcall JunoBender_midiValueChanged(MoogObject *o, double data, long userdata);
    friend void __fastcall JunoKeyboard_midiGateChanged(MoogObject *, double, long);
    friend void __fastcall JunoKeyboard_gtkKeyPressed(int, int, void *);
    friend void __fastcall JunoKeyboard_gtkKeyReleased(int, void *);
    friend void __fastcall JunoKeyboard_octaveTransposeChanged(MoogObject *, double, long);
    friend void __fastcall JunoKeyboard_keyTransposeChanged(MoogObject *, double, long);
    friend void __fastcall JunoKeyboard_masterTuneChanged(MoogObject *, double, long);
    friend void __fastcall JunoKeyboard_holdChanged(MoogObject *, double, long);
    friend void __fastcall Junokeyboard_changePatch(MoogObject *o, double data, long userdata);

    int initz; //what is this for ???

    int octaveTranspose;
    int keyTransposePressed;
    int keyTranspose;
    int holdPressed;
    int numVoices;
    double masterTune;

    int *savedGateInfo;
    Output **pitchOutputs;
    Output **gateOutputs;
/*
    void midiGateChanged(int, double);
    void gtkKeyPressed(unsigned int, unsigned int);
    void gtkKeyReleased(unsigned int);
    void octaveTransposeChanged(double);
    void keyTransposeChanged(double);
    void masterTuneChanged(double);
    void holdChanged(double);
*/
    void changePatch();
    void changeBender(double);

    void transposeVoices(double);

    Control *control;
    MidiInput *midiInput;
    juno_patch *patches;

public:

    JunoKeyboard(

        Control *_control,
        MidiInput *_midiInput,
        juno_patch *_patches,

        int, Scheduler *sched, ConnectionManager *conn);

    ~JunoKeyboard();

    int getKeyTranspose()
    {
        return keyTranspose;
    }

    const char *getClassName()
    {
        return ("JunoKeyboard");
    }
};

#endif
