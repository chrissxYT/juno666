#ifndef _JUNO_KEYBOARD_H
#define _JUNO_KEYBOARD_H


#include <libmoog/moog.h>


class JunoKeyboard: public MoogObject
{
    friend void JunoKeyboard_midiGateChanged(MoogObject *, double, long);
    friend void JunoKeyboard_gtkKeyPressed(int, int, void *);
    friend void JunoKeyboard_gtkKeyReleased(int, void *);
    friend void JunoKeyboard_octaveTransposeChanged(MoogObject *, double, long);
    friend void JunoKeyboard_keyTransposeChanged(MoogObject *, double, long);
    friend void JunoKeyboard_masterTuneChanged(MoogObject *, double, long);
    friend void JunoKeyboard_holdChanged(MoogObject *, double, long);

    int octaveTranspose;
    int keyTransposePressed;
    int keyTranspose;
    int holdPressed;
    int numVoices;
    double masterTune;

    int *savedGateInfo;
    Output **pitchOutputs;
    Output **gateOutputs;

    void midiGateChanged(int, double);
    void gtkKeyPressed(unsigned int, unsigned int);
    void gtkKeyReleased(unsigned int);
    void octaveTransposeChanged(double);
    void keyTransposeChanged(double);
    void masterTuneChanged(double);
    void holdChanged(double);

    void transposeVoices(double);

public:
    
    JunoKeyboard(int _numVoices, Scheduler *sched, ConnectionManager *conn);
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


