#include <stdio.h>

#include "VstJuno6.h"

VstJuno6::VstJuno6 (audioMasterCallback audioMaster) : 
AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
    // initialize programs
    programs = new VstJuno6Program[kNumPrograms];

    for (long i = 0; i < 16; i++)
        channelPrograms[i] = i;

    if (programs)
        setProgram (0);
    
    if (audioMaster)
    {
        setNumInputs (0);
        setNumOutputs (kNumOutputs);
        canProcessReplacing ();
        hasVu (false);
        hasClip (false);
        isSynth ();
        setUniqueID ('Jun6');
    }
    initProcess ();
    suspend ();
}

VstJuno6::~VstJuno6 ()
{
    if (programs)
        delete[] programs;
}

void VstJuno6::setProgram (long program)
{
    if (program < 0 || program >= kNumPrograms)
        return;
    
    VstJuno6Program currentProgram = &programs[program];
    curProgram = program;
}

void VstJuno6::setProgramName (char *name)
{
    strcpy (currentProgram->name, name);
}

void VstJuno6::getProgramName (char *name)
{
    strcpy (name, currentProgram->name);
}

void VstJuno6::getParameterLabel (long index, char *label)
{
    strcpy (label, " ");
}

void VstJuno6::getParameterDisplay (long index, char *text)
{
    text[0] = 0;

    float2string (control->getOutput(index)->getData(), text);
}

void VstJuno6::getParameterName (long index, char *label)
{
    strcpy(label, control->getOutput(index)->getName());
}

void VstJuno6::setParameter (long index, float value)
{
    value = control->getOutput(index)->setData(value);
}

float VstJuno6::getParameter (long index)
{
    return control->getOutput(index)->getData();
}

//-----------------------------------------------------------------------------------------
bool VstJuno6::getProgramNameIndexed (long category, long index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
bool VstJuno6::copyProgram (long destination)
{
    if (destination < kNumPrograms)
    {
        programs[destination] = programs[curProgram];
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
bool VstJuno6::getEffectName (char* name)
{
    strcpy (name, "VstJuno6");
    return true;
}

//-----------------------------------------------------------------------------------------
bool VstJuno6::getVendorString (char* text)
{
    strcpy (text, "Brainslayer");
    return true;
}

//-----------------------------------------------------------------------------------------
bool VstJuno6::getProductString (char* text)
{
    strcpy (text, "Juno 106");
    return true;
}

//-----------------------------------------------------------------------------------------
long VstJuno6::canDo (char* text)
{
    if (!strcmp (text, "receiveVstEvents"))
        return 1;
    if (!strcmp (text, "receiveVstMidiEvent"))
        return 1;
    if (!strcmp (text, "midiProgramNames"))
        return 1;
    return -1;  // explicitly can't do; 0 => don't know
}
