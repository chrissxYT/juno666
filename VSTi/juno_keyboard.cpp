/*
6.4. BrainSlayer

wrapper for keyboard functions, replaces GTK UI

*/


#include <libmoog/JunoControl.h>
#include <libmoog/MidiInput.h>
#include <libmoog/JunoPatch.h>
#include "juno_keyboard.h"

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

void JunoKeyboard_holdChanged(MoogObject *o, double data, long)
{
    ((JunoKeyboard *)o)->holdChanged(data);
}

void Junokeyboard_changePatch(MoogObject *o, double data, long userdata)
{
    ((JunoKeyboard *)o)->changePatch();
}

JunoKeyboard::JunoKeyboard(
JunoControl *_control,
MidiInput *_midiInput,
juno_patch *_patches,
int _numVoices, Scheduler *sched, ConnectionManager *conn): 
MoogObject(sched, conn),
control(_control),
midiInput(_midiInput),
patches(_patches),
numVoices(_numVoices)
{
    initz = 0;

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
        PATCH(midiInput, tmp1, this, tmp2);
        pitchOutputs[i] = control->getOutput(tmp2);

        tmp1.sprintf("amp%d", i);
        tmp2.sprintf("voice%d_gate", i);
        addInput(tmp2, JunoKeyboard_midiGateChanged, i, 1);
        PATCH(midiInput, tmp1, this, tmp2);
        gateOutputs[i] = control->getOutput(tmp2);
    }

    masterTune = 0;
    addInput("octave_transpose", JunoKeyboard_octaveTransposeChanged, 0, 1);
    addInput("transpose_switch", JunoKeyboard_keyTransposeChanged, 0, 1);
    addInput("master_tune", JunoKeyboard_masterTuneChanged, 0, 1);
    addInput("hold_switch", JunoKeyboard_holdChanged, 0, 1);
    addInput("patch_change", Junokeyboard_changePatch, 0, 1);

    PATCH(control, "octave_transpose", this, "octave_transpose");
    PATCH(control, "transpose_switch", this, "transpose_switch");
    PATCH(control, "master_tune", this, "master_tune");
    PATCH(control, "hold_switch", this, "hold_switch");
    PATCH(control, "patch_change", this, "patch_change");

    initz = 1;
}

JunoKeyboard::~JunoKeyboard()
{
    delete[]savedGateInfo;
    delete[]pitchOutputs;
    delete[]gateOutputs;
}

void JunoKeyboard::midiGateChanged(int voiceNum, double data)
{
    if (!initz)
        return;
    //printf("%f\n",inputs[voiceNum * 2].data);
    if (data > 0)
    {
        JunoKeyboard::gtkKeyPressed(voiceNum, pitch_to_note(schedule->nyquistFreq * *inputs[voiceNum * 2].data) - 36);

    }
    else
    {
        JunoKeyboard::gtkKeyReleased(voiceNum);
    }

}

void JunoKeyboard::gtkKeyPressed(unsigned int voice, unsigned int key)
{
    if (!initz)
        return;
    if (keyTransposePressed)
    {
        transposeVoices(pow((double)2, (double)(((double)(key % 12) - keyTranspose) / 12)));
        keyTranspose = key % 12;
    }
    else
    {
        double val = CPS(midi_notes[key+36 - octaveTranspose * 12 + keyTranspose]) * pow((double)2, (double)(masterTune / 12));
        pitchOutputs[voice]->setData(val);
        gateOutputs[voice]->setData(1.0);

        savedGateInfo[voice] = 1;
    }
}

void JunoKeyboard::gtkKeyReleased(unsigned int voice)
{
    if (!initz)
        return;
    if (!holdPressed)
    {

        if (gateOutputs[voice] == NULL)
            puts("bad condition");

        gateOutputs[voice]->setData(0.0);
    }

    savedGateInfo[voice] = 0;
}

void JunoKeyboard::octaveTransposeChanged(double data)
{
    if (!initz)
        return;
    //printf("octave transpose changed to %f\n", data);

    int octaves = (int)octaveTranspose - (int)data;
    octaveTranspose = (int)data;

    transposeVoices(pow((double)2, (double)octaves));
}

void JunoKeyboard::keyTransposeChanged(double data)
{
    if (!initz)
        return;
    //printf("key transpose changed to %f\n", data);
    keyTransposePressed = (data) ? 1 : 0;
}


void JunoKeyboard::masterTuneChanged(double data)
{
    if (!initz)
        return;
    data = data * 2 - 1;
    double adjustment = pow((double)2, (double)((data - masterTune) / 12.0));
    masterTune = data;

    //printf("master tune changed to %f adjustment=%f\n", data, adjustment);
    transposeVoices(adjustment);
}

void JunoKeyboard::holdChanged(double data)
{
    if (!initz)
        return;
    //printf("hold changed to %f\n", data);
    if (!(holdPressed = (data) ? 1 : 0))
    {
        for (int i = 0;i < numVoices;i++)
            if (!savedGateInfo[i])
                gateOutputs[i]->setData(0.0);
    }
}

void JunoKeyboard::transposeVoices(double tune)
{
    if (!initz)
        return;
    //printf( "TRANSPOSE BY %f\n", tune );
    for (int i = 0;i < numVoices;i++)
        pitchOutputs[i]->setData(pitchOutputs[i]->data * tune);
}

void JunoKeyboard::changePatch()
{
    if (!initz)
        return;

    int patchId = (int) * control->MoogObject::getOutput("patch_change")->getData();

    if (patchId > NUM_PATCHES)
    {
        printf("can't change. highest patch number is %d\n", NUM_PATCHES);
        return;
    }
    juno_patch *patch = &patches[patchId];
    control->MoogObject::getOutput("bender_dco")->setData(patch->bender_dco);
    control->MoogObject::getOutput("bender_vcf")->setData(patch->bender_vcf);
    control->MoogObject::getOutput("lfo_trigger")->setData(patch->lfo_trigger);
    control->MoogObject::getOutput("volume")->setData(patch->volume);
    control->MoogObject::getOutput("octave_transpose")->setData(patch->octave_transpose);
    control->MoogObject::getOutput("master_tune");
    control->MoogObject::getOutput("transpose_switch");
    control->MoogObject::getOutput("hold_switch");
    control->MoogObject::getOutput("arpeggio_switch")->setData(patch->arpeggio_switch);
    control->MoogObject::getOutput("arpeggio_mode")->setData(patch->arpeggio_mode);
    control->MoogObject::getOutput("arpeggio_range")->setData(patch->arpeggio_range);
    control->MoogObject::getOutput("arpeggio_rate")->setData(patch->arpeggio_rate);
    control->MoogObject::getOutput("lfo_rate")->setData(patch->lfo_rate);
    control->MoogObject::getOutput("lfo_delay")->setData(patch->lfo_delay);
    control->MoogObject::getOutput("lfo_mode")->setData(patch->lfo_mode);
    control->MoogObject::getOutput("dco_lfo")->setData(patch->dco_lfo);
    control->MoogObject::getOutput("dco_pwm")->setData(patch->dco_pwm);
    control->MoogObject::getOutput("dco_pwm_mod")->setData(patch->dco_pwm_mod);
    control->MoogObject::getOutput("dco_pulse_switch")->setData(patch->dco_pulse_switch);
    control->MoogObject::getOutput("dco_saw_switch")->setData(patch->dco_saw_switch);
    control->MoogObject::getOutput("dco_sub_switch")->setData(patch->dco_sub_switch);
    control->MoogObject::getOutput("dco_sub")->setData(patch->dco_sub);
    control->MoogObject::getOutput("dco_noise")->setData(patch->dco_noise);
    control->MoogObject::getOutput("hpf_frq")->setData(patch->hpf_frq);
    control->MoogObject::getOutput("vcf_frq")->setData(patch->vcf_frq);
    control->MoogObject::getOutput("vcf_res")->setData(patch->vcf_res);
    control->MoogObject::getOutput("vcf_env_invert")->setData(patch->vcf_env_invert);
    control->MoogObject::getOutput("vcf_env")->setData(patch->vcf_env);
    control->MoogObject::getOutput("vcf_lfo")->setData(patch->vcf_lfo);
    control->MoogObject::getOutput("vcf_kbd")->setData(patch->vcf_kbd);
    control->MoogObject::getOutput("vca_mode")->setData(patch->vca_mode);
    control->MoogObject::getOutput("env_attack")->setData(patch->env_attack);
    control->MoogObject::getOutput("env_decay")->setData(patch->env_decay);
    control->MoogObject::getOutput("env_sustain")->setData(patch->env_sustain);
    control->MoogObject::getOutput("env_release")->setData(patch->env_release);
    control->MoogObject::getOutput("chorus_I_switch")->setData(patch->chorus_I_switch);
    control->MoogObject::getOutput("chorus_II_switch")->setData(patch->chorus_II_switch);
    control->MoogObject::getOutput("panning")->setData(patch->panning);
}
