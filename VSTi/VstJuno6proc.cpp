#include "VstJuno6.h"

ConnectionManager* VstJuno6::getConnectionManager()
{
    return connection;
}

void VstJuno6::setSampleRate(float sampleRate)
{
    AudioEffectX::setSampleRate(sampleRate);

    schedule->setSampleRate((int)sampleRate);
}

void VstJuno6::setBlockSize(long blockSize)
{
    AudioEffectX::setBlockSize(blockSize);
}

void VstJuno6::resume()
{
    wantEvents();
}

void VstJuno6::initSynth(int numVoices)
{
    dsp = new VSTOutput(control, schedule, connection);

    lfo = new JunoLfo(control, numVoices, schedule, connection);

    pwmLfo = new Attenuator(schedule);

    noise = new Rand(schedule);

    voiceMix = new Mixer(schedule, connection, numVoices);

    hpf = new HPF(schedule);

    PATCH(lfo, "sig", pwmLfo, "sig");

    pwmLfo->set("amp", .5);

    pwmLfo->set("zro", .5);

    /* fall thru each case */
    if (numVoices > 64)
    {
        numVoices = 64;
    }

    for (int i = 0;i < numVoices;i++)
    {
        //debug(DEBUG_APPMSG1, "CREATING VOICE %d",i);
        voice[i] = new JunoVoice(control, i, noise, lfo, pwmLfo, schedule, connection);
        char *tmp = new char[6];
        sprintf(tmp, "sig%d", i);
        //debug(DEBUG_APPMSG1, "patching voice",i);
        PATCH(voice[i], "sig", voiceMix, tmp);
        //voice[i]->attachVoice(midiInput);
    }

    arpeggio = new JunoArpeggio(control, numVoices, voice, schedule, connection);

    PATCH(voiceMix, "sig", hpf, "sig");

    PATCH(control, "hpf_frq", hpf, "frq");
    //FIXME: tune the Q of the hpf

    hpf->set("Q", 0.0);

    chorus = new JunoChorus(hpf, "sig", 0);

    PATCH(control, "chorus_off_switch", chorus, "off");
    PATCH(control, "chorus_I_switch", chorus, "I");
    PATCH(control, "chorus_II_switch", chorus, "II");

    PATCH(chorus, "sig", dsp, "sig0");
    PATCH(control, "volume", dsp, "amp0");

    // for stereo
    chorus2 = new JunoChorus(hpf, "sig", 1);
    PATCH(control, "chorus_off_switch", chorus2, "off");
    PATCH(control, "chorus_I_switch", chorus2, "I");
    PATCH(control, "chorus_II_switch", chorus2, "II");

    PATCH(chorus2, "sig", dsp, "sig1");
    PATCH(control, "volume", dsp, "amp1");

    for (int i = 0;i < numVoices;i++)
    {
        control->getOutput((String)"voice" + i + (String)"_pitch")->setData(CPS(666));
    }
}

void VstJuno6::initProcess()
{
    int numVoices = 6;

    control = new JunoControl(numVoices, schedule);

    midiInput = new MidiInput(control, numVoices, schedule);

    initSynth(numVoices);

    patches = juno_patchset_new();

    String patchFileName = "juno6.patches";

    load_patches(patchFileName, patches);

    keyboard = new JunoKeyboard(control, midiInput, patches, numVoices, schedule, connection);

    control->MoogObject::getOutput("patch_change")->setData(0);

}

void VstJuno6::process(float **inputs, float **outputs, long sampleFrames)
{
    processReplacing(inputs, outputs, sampleFrames);
}

/*
FILE* verbose;

verbose = fopen ( "debug_dump", "a+" );

fprintf ( verbose, "loop %d\n", sampleFrames);

fclose ( verbose );
*/

void VstJuno6::processReplacing(float **inputs, float **outputs, long sampleFrames)
{
    float *out1 = outputs[0];
    float *out2 = outputs[1];

    if (midiInput->isNoteOn())
    {
        if (currentDelta > 0)
        {
            if (currentDelta >= sampleFrames) // future
            {
                currentDelta -= sampleFrames;
                return;
            }
            memset(out1, 0, currentDelta * sizeof(float));
            memset(out2, 0, currentDelta * sizeof(float));
            out1 += currentDelta;
            out2 += currentDelta;
            sampleFrames -= currentDelta;
            currentDelta = 0;
        }

        dsp->setOutput1(out1);

        dsp->setOutput2(out2);

        while (--sampleFrames >= 0)
        {

            schedule->run();
        }

    }
    else
    {
        memset(out1, 0, sampleFrames * sizeof(float));
        memset(out2, 0, sampleFrames * sizeof(float));
    }
}

long VstJuno6::processEvents(VstEvents *ev)
{
    unsigned char cmd = 0;
    unsigned char channel = 0;
    unsigned char data[2];
    VstMidiEvent *event;
    char *midiData;

    for (long i = 0;i < ev->numEvents;i++)
    {
        if ((ev->events[i])->type != kVstMidiType)
            continue;

        event = (VstMidiEvent *)ev->events[i];
        midiData = event->midiData;

        cmd = midiData[0] & 0xf0;
        channel = midiData[0] & 0x0f;
        data[0] = midiData[1] & 0xff;
        data[1] = midiData[2] & 0xff;

        midiInput->proc(cmd, channel, data);
    }

    return 1;
}
