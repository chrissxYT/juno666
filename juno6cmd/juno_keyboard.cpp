/*
6.4. BrainSlayer

wrapper for keyboard functions, replaces GTK UI

*/


#include <libmoog/JunoControl.h>
#include <libmoog/JunoPatch.h>
#include "juno_keyboard.h"

extern Settings *settings;
extern JunoKeyboard *keyboard;
extern juno_patch *patches;


void JunoKeyboard_midiGateChanged(MoogObject *o, double data, long userData)
{
    ((JunoKeyboard *)o)->midiGateChanged(userData, data);
}

void JunoKeyboard_gtkKeyPressed(int voice, int key, void *data)
{
    ((JunoKeyboard *)data)->gtkKeyPressed(voice, key);
}

void JunoKeyboard_gtkKeyReleased(int voice, void *data)
{
    ((JunoKeyboard *)data)->gtkKeyReleased(voice);
}

void JunoKeyboard_octaveTransposeChanged(MoogObject *o, double data, long)
{
    ((JunoKeyboard *)o)->octaveTransposeChanged(data);
}

void JunoKeyboard_keyTransposeChanged(MoogObject *o, double data, long)
{
    ((JunoKeyboard *)o)->keyTransposeChanged(data);
}

void JunoKeyboard_masterTuneChanged(MoogObject *o, double data, long)
{
    ((JunoKeyboard *)o)->masterTuneChanged(data);
}

JunoControl *junoControl;
void Junokeyboard_changePatch(MoogObject *o, double data, long userdata)
{

    userdata = (int) * junoControl->MoogObject::getOutput("patch_change")->getData();

    if (userdata > NUM_PATCHES)
    {
        printf("can't change. highest patch number is %d\n", NUM_PATCHES);
        return;
    }

    juno_patch *patch =&patches[userdata];
    junoControl->MoogObject::getOutput("bender_dco")->setData(patch->bender_dco);
    junoControl->MoogObject::getOutput("bender_vcf")->setData(patch->bender_vcf);
    junoControl->MoogObject::getOutput("lfo_trigger")->setData(patch->lfo_trigger);
    junoControl->MoogObject::getOutput("volume")->setData(patch->volume);
    junoControl->MoogObject::getOutput("octave_transpose")->setData(patch->octave_transpose);
    junoControl->MoogObject::getOutput("master_tune");
    junoControl->MoogObject::getOutput("transpose_switch");
    junoControl->MoogObject::getOutput("hold_switch");
    junoControl->MoogObject::getOutput("arpeggio_switch")->setData(patch->arpeggio_switch);
    junoControl->MoogObject::getOutput("arpeggio_mode")->setData(patch->arpeggio_mode);
    junoControl->MoogObject::getOutput("arpeggio_range")->setData(patch->arpeggio_range);
    junoControl->MoogObject::getOutput("arpeggio_rate")->setData(patch->arpeggio_rate);
    junoControl->MoogObject::getOutput("lfo_rate")->setData(patch->lfo_rate);
    junoControl->MoogObject::getOutput("lfo_delay")->setData(patch->lfo_delay);
    junoControl->MoogObject::getOutput("lfo_mode")->setData(patch->lfo_mode);
    junoControl->MoogObject::getOutput("dco_lfo")->setData(patch->dco_lfo);
    junoControl->MoogObject::getOutput("dco_pwm")->setData(patch->dco_pwm);
    junoControl->MoogObject::getOutput("dco_pwm_mod")->setData(patch->dco_pwm_mod);
    junoControl->MoogObject::getOutput("dco_pulse_switch")->setData(patch->dco_pulse_switch);
    junoControl->MoogObject::getOutput("dco_saw_switch")->setData(patch->dco_saw_switch);
    junoControl->MoogObject::getOutput("dco_sub_switch")->setData(patch->dco_sub_switch);
    junoControl->MoogObject::getOutput("dco_sub")->setData(patch->dco_sub);
    junoControl->MoogObject::getOutput("dco_noise")->setData(patch->dco_noise);
    junoControl->MoogObject::getOutput("hpf_frq")->setData(patch->hpf_frq);
    junoControl->MoogObject::getOutput("vcf_frq")->setData(patch->vcf_frq);
    junoControl->MoogObject::getOutput("vcf_res")->setData(patch->vcf_res);
    junoControl->MoogObject::getOutput("vcf_env_invert")->setData(patch->vcf_env_invert);
    junoControl->MoogObject::getOutput("vcf_env")->setData(patch->vcf_env);
    junoControl->MoogObject::getOutput("vcf_lfo")->setData(patch->vcf_lfo);
    junoControl->MoogObject::getOutput("vcf_kbd")->setData(patch->vcf_kbd);
    junoControl->MoogObject::getOutput("vca_mode")->setData(patch->vca_mode);
    junoControl->MoogObject::getOutput("env_attack")->setData(patch->env_attack);
    junoControl->MoogObject::getOutput("env_decay")->setData(patch->env_decay);
    junoControl->MoogObject::getOutput("env_sustain")->setData(patch->env_sustain);
    junoControl->MoogObject::getOutput("env_release")->setData(patch->env_release);
    junoControl->MoogObject::getOutput("chorus_I_switch")->setData(patch->chorus_I_switch);
    junoControl->MoogObject::getOutput("chorus_II_switch")->setData(patch->chorus_II_switch);
}

int initz=0;
JunoKeyboard::JunoKeyboard(int _numVoices, JunoControl *ctl, MidiInput *midi, Scheduler *sched, ConnectionManager *conn):
MoogObject(sched, conn),
numVoices(_numVoices)
{
    junoControl = ctl;
    octaveTranspose = -1;
    keyTransposePressed = 0;
    keyTranspose = 0;
    holdPressed = 0;
    savedGateInfo = new int[numVoices];
    pitchOutputs = new Output *[numVoices];
    gateOutputs = new Output *[numVoices];
    
    memset(savedGateInfo, 0, numVoices * sizeof(int));


    for (int i = 0;i < numVoices;i++)
    {
        String tmp1, tmp2;
        tmp1.sprintf("sig%d", i);
        tmp2.sprintf("voice%d_pitch", i);
        addInput(tmp2, NULL, 0, 0);
        PATCH(midi, tmp1, this, tmp2);
        pitchOutputs[i] = junoControl->getOutput(tmp2);

        tmp1.sprintf("amp%d", i);
        tmp2.sprintf("voice%d_gate", i);
        addInput(tmp2, JunoKeyboard_midiGateChanged, i, 1);
        PATCH(midi, tmp1, this, tmp2);
        gateOutputs[i] = junoControl->getOutput(tmp2);
    }

    masterTune = 0;
    addInput("octave_transpose", JunoKeyboard_octaveTransposeChanged, 0, 1);
    addInput("transpose_switch", JunoKeyboard_keyTransposeChanged, 0, 1);
    addInput("master_tune", JunoKeyboard_masterTuneChanged, 0, 1);
    addInput("patch_change", Junokeyboard_changePatch, 0, 1);

    PATCH(junoControl, "octave_transpose", this, "octave_transpose");
    PATCH(junoControl, "transpose_switch", this, "transpose_switch");
    PATCH(junoControl, "master_tune", this, "master_tune");
    PATCH(junoControl, "patch_change", this, "patch_change");
    initz=1;
}

JunoKeyboard::~JunoKeyboard()
{
    delete[]savedGateInfo;
    delete[]pitchOutputs;
    delete[]gateOutputs;
}





void JunoKeyboard::midiGateChanged(int voiceNum, double data)
{
if (!initz)return;
    //printf("%f\n",inputs[voiceNum * 2].data);
    if (data > 0)
    {
        JunoKeyboard::gtkKeyPressed(voiceNum,pitch_to_note(schedule->nyquistFreq * *inputs[voiceNum * 2].data) - 36);
    }
    else
    {
        JunoKeyboard::gtkKeyReleased(voiceNum);
    }
    
}

void JunoKeyboard::gtkKeyPressed(unsigned int voice, unsigned int key)
{
    //printf("----------------key %u was pressed on voice %u\n", key, voice);

    if (keyTransposePressed)
    {
        transposeVoices(pow((double)2, ((double)(key % 12) - keyTranspose) / 12));
        keyTranspose = key % 12;
    }
    else
    {
        double val = CPS(midi_notes[key+36 - octaveTranspose * 12 + keyTranspose]) * pow((double)2, masterTune / 12);
        pitchOutputs[voice]->setData(val);
        gateOutputs[voice]->setData(1.0);

        savedGateInfo[voice] = 1;
    }
}

void JunoKeyboard::gtkKeyReleased(unsigned int voice)
{
    //printf("----------------voice %u was released\n", voice);

    if (!holdPressed)
    {
        if (gateOutputs[voice]==NULL)puts("bad condition");
        
        gateOutputs[voice]->setData(0.0);
    }

    savedGateInfo[voice] = 0;
}

void JunoKeyboard::octaveTransposeChanged(double data)
{
    //printf("octave transpose changed to %f\n", data);

    int octaves = (int)octaveTranspose - (int)data;
    octaveTranspose = (int)data;

    transposeVoices(pow((double)2, (double)octaves));
}

void JunoKeyboard::keyTransposeChanged(double data)
{
    //printf("key transpose changed to %f\n", data);
    keyTransposePressed = (data) ? 1 : 0;
}


void JunoKeyboard::masterTuneChanged(double data)
{
    data = data * 2 - 1;
    double adjustment = pow((double)2, (data - masterTune) / 12.0);
    masterTune = data;

    //printf("master tune changed to %f adjustment=%f\n", data, adjustment);
    transposeVoices(adjustment);
}

void JunoKeyboard::transposeVoices(double tune)
{
    //printf( "TRANSPOSE BY %f\n", tune );
    for (int i = 0;i < numVoices;i++)
        pitchOutputs[i]->setData(pitchOutputs[i]->data * tune);
}