#include <stdio.h>

#include "VstJuno6.h"

extern JunoControl *control;
extern juno_patch *patches;
extern JunoKeyboard *keyboard;

VstJuno6::VstJuno6 (audioMasterCallback audioMaster) : 
AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
    setProgram (0);
	Scheduler::Init();   
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
	Scheduler::DeInit();
}

void VstJuno6::setProgram (long program)
{
    if (!control || program < 0 || program >= kNumPrograms)
        return;

    curProgram = program;

    control->MoogObject::getOutput("patch_change")->setData(program);
}

void VstJuno6::setProgramName (char *name)
{
    strncpy (patches[curProgram].name, name, PATCH_NAME_LEN);
}

void VstJuno6::getProgramName (char *name)
{
    strcpy (name, patches[curProgram].name);
}

void VstJuno6::getParameterLabel (long index, char *label)
{
    strcpy (label, "roentgen");
}

void VstJuno6::getParameterDisplay (long index, char *text)
{
    text[0] = 0;

    if(control)
    {
        float2string ((float)*control->getOutput(index)->getData(), text);
    }
}

void VstJuno6::getParameterName (long index, char *label)
{
    if(control)
    {
		const char *name = control->getOutput(index)->getDisplayName();
		if (name!=NULL)
			strcpy(label, name);
		else 
			strcpy(label,"");
    }
}

void VstJuno6::setParameter (long index, float value)
{
    if(control)
    {
        control->getOutput(index)->setData(value);
    }
}

float VstJuno6::getParameter (long index)
{
    if(control)
    {
        return (float)*control->getOutput(index)->getData();
    }
    else
    {
        return 0;
    }    
}

bool VstJuno6::getProgramNameIndexed (long category, long index, char* text)
{
    if (index < kNumPrograms)
    {
//        sprintf(text,"%4.4f",(float)*control->getOutput(0)->getData());

        strcpy (text, patches[index].name);
        return true;
    }
    return false;
}

bool VstJuno6::copyProgram (long destination)
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

bool VstJuno6::getEffectName (char* name)
{
    strcpy (name, "VstJuno6");
    return true;
}

bool VstJuno6::getVendorString (char* text)
{
    strcpy (text, "Brainslayer & Strepto");
    return true;
}

bool VstJuno6::getProductString (char* text)
{
    strcpy (text, "Juno 106");
    return true;
}

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
