/*
 * Copyright(c) 2000 UltraMaster Group
 *
 * License to use UltraMaster Juno-6 is provided free of charge subject to the
 * following restrictions:
 *
 * 1.) This license is for your personal use only.
 *
 * 2.) No portion of this software may be redistributed by you to any other
 *     person in any form.
 *
 * 3.) You may not sell UltraMaster Juno-6 to any person.
 *
 * 4.) UltraMaster Juno-6 is provided without any express or implied warranty.
 *     In no event shall UltraMaster Group be held liable for any damages
 *     arising from the use of UltraMaster Juno-6.
 */
/**
 * Copyright (c) UltraMaster Group, LLC. All Rights Reserved.
 * $Revision: 1.6 $$Date: 2004/04/21 09:57:56 $
 */
#include <stdlib.h>
#include <libmoog/JunoSynth.h>
#include <libmoog/JunoVoice.h>
#include <libmoog/JunoLfo.h>
#include <libmoog/JunoArpeggio.h>
#include <libmoog/JunoChorus.h>
#include <libmoog/JunoPatch.h>

//FIXME: this is grosser than gross


DSPOutput *dsp;
int numV;
JunoVoice *voice[64];

Mixer *voiceMix;
Rand *noise;
JunoLfo *lfo;
JunoArpeggio *arpeggio;
JunoChorus *chorus;
JunoChorus *chorus2;
Attenuator *pwmLfo;
HPF *hpf;

extern Settings *settings;
extern String patchFileName;



bool stereo;
static ConnectionManager *conn;
ConnectionManager* 
getConnectionManager()
{
    return conn;
}

void
setScope(int v)
{
//just do nothing

}

void
initSynth(JunoControl *junoControl,
    Settings *settings,
    MidiInput *midiInput,
	ConnectionManager *connection,
	Scheduler *schedule,
    int numVoices)
{
	conn = connection;
    stereo = strcmp("yes",
        settings->getString("devices", "stereo_output")) == 0;

    int numFrags = -1;
    int fragSize = -1;
    const char *p;

    p = settings->getString("devices", "NumFrags");
    if (strlen(p))
        numFrags = atoi(p);

    p = settings->getString("devices", "FragSize");
    if (strlen(p))
        fragSize = atoi(p);

    //FIXME: we have to hold on to the junoControl pointer... sloppy
    puts("copy control");
//    junoControl = _junoControl;

    puts("new dsp");
    dsp = new DSPOutput(junoControl, schedule, connection, settings->getString("devices", "dsp-output"),
        SAMPLE_RATE_44k,
        (stereo) ? 2 : 1,
        numFrags,
        fragSize);

    puts("new lfo");
    lfo = new JunoLfo(junoControl, numVoices, schedule, connection);

    pwmLfo = new Attenuator(schedule);

    noise = new Rand(schedule);

    voiceMix = new Mixer(schedule, connection, numVoices);

    hpf = new HPF(schedule);

    PATCH(lfo, "sig", pwmLfo, "sig");

    pwmLfo->set("amp", .5);

    pwmLfo->set("zro", .5);

    //FIXME: an array of voices would be nice, huh?
    debug(DEBUG_APPMSG1, "Creating %d voices...", numVoices);

    /* fall thru each case */
    if (numVoices > 64)
    {
        debug(DEBUG_APPMSG1, "more than 64 voices are not allowed, truncated!");
        numVoices = 64;
    }
    numV = numVoices;
    for (int i = 0;i < numVoices;i++)
    {
        //debug(DEBUG_APPMSG1, "CREATING VOICE %d",i);
        voice[i] = new JunoVoice(junoControl, i, noise, lfo, pwmLfo, schedule, connection);
        char *tmp = new char[6];
        sprintf(tmp, "sig%d", i);
        //debug(DEBUG_APPMSG1, "patching voice",i);
        PATCH(voice[i], "sig", voiceMix, tmp);
        //voice[i]->attachVoice(midiInput);
    }

    puts("new arpeggio");
    arpeggio = new JunoArpeggio(junoControl, numVoices, voice, schedule, connection);

    PATCH(voiceMix, "sig", hpf, "sig");

    PATCH(junoControl, "hpf_frq", hpf, "frq");
    //FIXME: tune the Q of the hpf

    hpf->set("Q", 0.0);

    chorus = new JunoChorus(hpf, "sig", 0);

    if (stereo)
    {

        chorus2 = new JunoChorus(hpf, "sig", 1);
        PATCH(junoControl, "chorus_off_switch", chorus2, "off");
        PATCH(junoControl, "chorus_I_switch", chorus2, "I");
        PATCH(junoControl, "chorus_II_switch", chorus2, "II");
        PATCH(chorus2, "sig", dsp, "sig1");
        PATCH(junoControl, "volume", dsp, "amp1");
    }

    PATCH(junoControl, "chorus_off_switch", chorus, "off");
    PATCH(junoControl, "chorus_I_switch", chorus, "I");
    PATCH(junoControl, "chorus_II_switch", chorus, "II");
    PATCH(chorus, "sig", dsp, "sig0");

    PATCH(junoControl, "volume", dsp, "amp0");

    // all cool dave, scope does nothing until you pop it up.
    //scope = new Scope();
    //PATCH(chorus, "sig", scope, "sig");

    /*if (stereo)
    {
        PATCH(chorus2, "sig", scope, "sig2");
    }*/

    /* the Balance in the voices needs signal to calibrate, so start
     * the oscillators with an arbitrary frequency - they run forever
     */
    for (int i = 0;i < numV;i++)
    {
        junoControl->getOutput((String)"voice" + i + (String)"_pitch")->setData(CPS(666));
    }

    PATCH(junoControl, "volume", dsp, "amp0");

    schedule->start(dsp);
}
