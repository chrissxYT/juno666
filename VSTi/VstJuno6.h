#ifndef __VstJuno6_H__
#define __VstJuno6_H__

#include "audioeffectx.h"
#include "VstJuno6Program.h"

//#include <libmoog/moog.h>

#include <libmoog/JunoVoice.h>
#include <libmoog/JunoLfo.h>
#include <libmoog/JunoArpeggio.h>
#include <libmoog/JunoChorus.h>
#include <libmoog/JunoPatch.h>
#include <libmoog/HPF.h>

#include "libmoog/MidiInput.h"
#include "libmoog/VSTOutput.h"

#include "juno_keyboard.h"

enum
{
    // Global
    kNumPrograms = 128,
    kNumOutputs = 2,

    // Parameters Tags
    kBender = 0,
    kBender_dco,
    kBender_vcf,
    kLfo_trigger,
    kVolume,  
    kOctave_transpose,
    kMaster_tune,
    kTranspose_switch,
    kHold_switch,
    kArpeggio_switch,
    kArpeggio_mode,
    kArpeggio_range,
    kArpeggio_rate,
    kLfo_rate,
    kLfo_delay,
    kLfo_mode,
    kDco_lfo,
    kDco_pwm,
    kDco_pwm_mod,
    kDco_pulse_switch,
    kDco_saw_switch,
    kDco_sub_switch,
    kDco_sub,
    kDco_noise,
    kHpf_frq, 
    kVcf_frq, 
    kVcf_res, 
    kVcf_env_invert,
    kVcf_env,
    kVcf_lfo,
    kVcf_kbd,
    kVca_mode,
    kEnv_attack,
    kEnv_decay,
    kEnv_sustain,
    kEnv_release,
    kChorus_off_switch,
    kChorus_I_switch, 
    kChorus_II_switch,

    kNumParams
};

class VstJuno6 : public AudioEffectX
{
public:
    VstJuno6 (audioMasterCallback audioMaster);
    ~VstJuno6 ();

    virtual void process (float **inputs, float **outputs, long sampleframes);
    virtual void processReplacing (float **inputs, float **outputs, long sampleframes);
    virtual long processEvents (VstEvents* events);

    virtual void setProgram (long program);
    virtual void setProgramName (char *name);
    virtual void getProgramName (char *name);
    virtual bool getProgramNameIndexed (long category, long index, char* text);
    virtual bool copyProgram (long destination);

    virtual void setParameter (long index, float value);
    virtual float getParameter (long index);
    virtual void getParameterLabel (long index, char *label);
    virtual void getParameterDisplay (long index, char *text);
    virtual void getParameterName (long index, char *text);
    
    virtual void setSampleRate (float sampleRate);
    virtual void setBlockSize (long blockSize);
    
    virtual void resume ();

    virtual bool getEffectName (char* name);
    virtual bool getVendorString (char* text);
    virtual bool getProductString (char* text);
    virtual long getVendorVersion () { return 1000; }
    virtual long canDo (char* text);

    void initSynth(int numVoices);

private:
    void initProcess ();

    VstJuno6Program* programs;

    VstJuno6Program* currentProgram;

    long channelPrograms[16];

    long currentDelta;

    VSTOutput *dsp;

    Mixer *voiceMix;
    JunoChorus *chorus;
    JunoChorus *chorus2;
    HPF *hpf;
};

#endif /* __VstJuno6_H__ */
