#include <stdio.h>

#include "VstJuno6.h"

#include "editor.h"
#include "patches.h"


VstJuno6::VstJuno6(audioMasterCallback audioMaster):
AudioEffectX(audioMaster, kNumPrograms, kNumParams)
{
    numVoices = 6;

    voice = new JunoVoice *[numVoices];

    schedule = new Scheduler();

    connection = new ConnectionManager();

    control = new JunoControl(numVoices, schedule);

    midiInput = new MidiInput(control, numVoices, schedule);
 
    initSynth(numVoices);

    patches = (juno_patch*)&junofactory;

    keyboard = new JunoKeyboard(control, midiInput, patches, numVoices, schedule, connection);

    setEditor( new Editor( this, control, midiInput ) );

    setProgram(0);

    if (audioMaster)
    {
        setNumInputs(0);
        setNumOutputs(kNumOutputs);
        canProcessReplacing();
        hasVu(false);
        hasClip(false);
        isSynth();
        setUniqueID('Jun6');
    }
    suspend();
}

VstJuno6::~VstJuno6()
{
    delete schedule;
    delete connection;
    delete control;
    delete midiInput;

}

void VstJuno6::setProgram(long program)
{
    if (!control || program < 0 || program >= kNumPrograms)
        return;

    curProgram = program;

    control->MoogObject::getOutput("patch_change")->setData(program);

    // Adjust the editor to the new program
    if (editor)
    {   
        for(int i=0; i<control->getNumOutputs(); i++)
        {
            ((AEffGUIEditor*)editor)->setParameter(i, getParameter(i));
        }
    }
}

void VstJuno6::setProgramName(char *name)
{
    strncpy(patches[curProgram].name, name, PATCH_NAME_LEN);
}

void VstJuno6::getProgramName(char *name)
{
    strcpy(name, patches[curProgram].name);
}

void VstJuno6::getParameterLabel(long index, char *label)
{
    strcpy(label, "roentgen");
}

void VstJuno6::getParameterDisplay(long index, char *text)
{
    text[0] = 0;

    if (control)
    {
        float2string((float) * control->getOutput(index)->getData(), text);
    }
}

void VstJuno6::getParameterName(long index, char *label)
{
    if (control)
    {
        const char *name = control->getOutput(index)->getDisplayName();
        if (name != NULL)
            strcpy(label, name);
        else
            strcpy(label, "");
    }
}

void VstJuno6::setParameter(long index, float value)
{
    if (editor)
        ((AEffGUIEditor*)editor)->setParameter (index, value);

    if (control)
    {
        switch (control->getOutput(index)->getType())
        {
            case WAY2:
                if (value > 0.1)
                    value = 1;
                else
                    if (value < 0.9)
                        value = 0;
                control->getOutput(index)->setData(value);
                break;
            case WAY3:
                if (value < 0.4)
                    value = -1;
                else
                    if (value > 0.6)
                        value = 1;
                    else
                        value = 0;
                control->getOutput(index)->setData(value);
                break;
            default:
                control->getOutput(index)->setData(value);
        }
    }
}

float VstJuno6::getParameter(long index)
{
    if (control)
    {
        float value = (float) * control->getOutput(index)->getData();
        switch (control->getOutput(index)->getType())
        {
            case WAY2:
                return value;
                break;
            case WAY3:
                value += 1;
                value /= 2;
                return value;
                break;
            default:
                return value;

        }
        return value;
    }

    return 0;
}

bool VstJuno6::getProgramNameIndexed(long category, long index, char *text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, patches[index].name);
        return true;
    }
    return false;
}

bool VstJuno6::copyProgram(long destination)
{
/*
    if (destination < kNumPrograms)
    {
    programs[destination] = programs[curProgram];
    return true;
    }
*/
    return false;
}

bool VstJuno6::getEffectName(char *name)
{
    strcpy(name, "VstJuno6");
    return true;
}

bool VstJuno6::getVendorString(char *text)
{
    strcpy(text, "Brainslayer and Strepto");
    return true;
}

bool VstJuno6::getProductString(char *text)
{
    strcpy(text, "Juno 60");
    return true;
}

long VstJuno6::canDo(char *text)
{
    if (!strcmp(text, "receiveVstEvents"))
        return 1;
    if (!strcmp(text, "receiveVstMidiEvent"))
        return 1;
    if (!strcmp(text, "midiProgramNames"))
        return 1;
    return -1; // explicitly can't do; 0 => don't know
}
