#ifndef _JUNO_KEYBOARD_H
#define _JUNO_KEYBOARD_H


//#include <libmoog/moog.h>

#include <libmoog/ADSR.h>
#include <libmoog/Attenuator.h>
#include <libmoog/Balance.h>
#include <libmoog/Chorus.h>
#include <libmoog/ConnectionInfo.h>
#include <libmoog/ConnectionList.h>
#include <libmoog/ConnectionManager.h>
#include <libmoog/DataBlock.h>
#include <libmoog/Delay.h>
#include <libmoog/GoObject.h>
#include <libmoog/HPF.h>
#include <libmoog/IIR2.h>
#include <libmoog/Input.h>
#include <libmoog/JunoPulse.h>
#include <libmoog/JunoSaw.h>
#include <libmoog/Mixer.h>
#include <libmoog/MoogObject.h>
#include <libmoog/Oscillator.h>
#include <libmoog/Output.h>
#include <libmoog/Rand.h>
#include <libmoog/ResonantLowPass.h>
#include <libmoog/Rms.h>
#include <libmoog/Settings.h>
#include <libmoog/WaveGen.h>
#include <libmoog/pitch.h>

#include <libmoog/Scheduler.h>



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
    
    JunoKeyboard(int);
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


