#include "VstJuno6.h"

enum
{
    kNumFrequencies = 128,  // 128 midi notes
    kWaveSize = 4096        // samples (must be power of 2 here)
};

//-----------------------------------------------------------------------------------------
void VstJuno6::setSampleRate (float sampleRate)
{
    AudioEffectX::setSampleRate (sampleRate);
    fScaler = (float)((double)kWaveSize / (double)sampleRate);
}

//-----------------------------------------------------------------------------------------
void VstJuno6::setBlockSize (long blockSize)
{
    AudioEffectX::setBlockSize (blockSize);
}

//-----------------------------------------------------------------------------------------
void VstJuno6::resume ()
{
    wantEvents ();
}

//-----------------------------------------------------------------------------------------
void VstJuno6::initProcess ()
{

}

//-----------------------------------------------------------------------------------------
void VstJuno6::process (float **inputs, float **outputs, long sampleFrames)
{
    processReplacing (inputs, outputs, sampleFrames);
}

//-----------------------------------------------------------------------------------------
void VstJuno6::processReplacing (float **inputs, float **outputs, long sampleFrames)
{
    float* out1 = outputs[0];
    float* out2 = outputs[1];
    if (noteIsOn)
    {
        float baseFreq = freqtab[currentNote & 0x7f] * fScaler;
        float freq1 = baseFreq + fFreq1;    // not really linear...
        float freq2 = baseFreq + fFreq2;
        float* wave1 = (fWaveform1 < .5) ? sawtooth : pulse;
        float* wave2 = (fWaveform2 < .5) ? sawtooth : pulse;
        float wsf = (float)kWaveSize;
        float vol = (float)(fVolume * (double)currentVelocity * midiScaler);
        long mask = kWaveSize - 1;
        
        if (currentDelta > 0)
        {
            if (currentDelta >= sampleFrames)   // future
            {
                currentDelta -= sampleFrames;
                return;
            }
            memset (out1, 0, currentDelta * sizeof (float));
            memset (out2, 0, currentDelta * sizeof (float));
            out1 += currentDelta;
            out2 += currentDelta;
            sampleFrames -= currentDelta;
            currentDelta = 0;
        }

        // loop
        while (--sampleFrames >= 0)
        {
            // this is all very raw, there is no means of interpolation,
            // and we will certainly get aliasing due to non-bandlimited
            // waveforms. don't use this for serious projects...
            (*out1++) = wave1[(long)fPhase1 & mask] * fVolume1 * vol;
            (*out2++) = wave2[(long)fPhase2 & mask] * fVolume2 * vol;
            fPhase1 += freq1;
            fPhase2 += freq2;
        }
    }                       
    else
    {
        memset (out1, 0, sampleFrames * sizeof (float));
        memset (out2, 0, sampleFrames * sizeof (float));
    }
}

//-----------------------------------------------------------------------------------------
long VstJuno6::processEvents (VstEvents* ev)
{
    for (long i = 0; i < ev->numEvents; i++)
    {
        if ((ev->events[i])->type != kVstMidiType)
            continue;
        VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
        char* midiData = event->midiData;
        long status = midiData[0] & 0xf0;       // ignoring channel
        if (status == 0x90 || status == 0x80)   // we only look at notes
        {
            long note = midiData[1] & 0x7f;
            long velocity = midiData[2] & 0x7f;
            if (status == 0x80)
                velocity = 0;   // note off by velocity 0
            if (!velocity && (note == currentNote))
                noteOff ();
            else
                noteOn (note, velocity, event->deltaFrames);
        }
        else if (status == 0xb0)
        {
            if (midiData[1] == 0x7e || midiData[1] == 0x7b) // all notes off
                noteOff ();
        }
        event++;
    }
    return 1;   // want more
}

//-----------------------------------------------------------------------------------------
void VstJuno6::noteOn (long note, long velocity, long delta)
{
    currentNote = note;
    currentVelocity = velocity;
    currentDelta = delta;
    noteIsOn = true;
    fPhase1 = fPhase2 = 0;
}

//-----------------------------------------------------------------------------------------
void VstJuno6::noteOff ()
{
    noteIsOn = false;
}
