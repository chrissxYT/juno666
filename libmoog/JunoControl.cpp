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
 * $Revision: 1.13 $$Date: 2004/06/10 09:13:58 $
 */

#include <libmoogutil/String.h>
#include "JunoControl.h"

//types

JunoControl::JunoControl(int voices, Scheduler *sched): MoogObject(sched, NULL)
{
    addOutput("bender", "Pitch Bender", false);
    addOutput("bender_dco", "DCO", false);
    addOutput("bender_vcf", "VCF", false);
    addOutput("lfo_trigger", "LFO Button", false);
    addOutput("volume", "Volume", false);
    addOutput("octave_transpose", "Transpose", WAY3, false);

    addOutput("master_tune", "Tune", false);

    addOutput("transpose_switch", "Transpose SW", WAY2, false); // 0 = normal, non zero = transpose
    addOutput("hold_switch", "Hold", WAY2, false); // 0 = normal, non zero = hold

    addOutput("arpeggio_switch", "Arpeggio On", WAY2, false); // 0 = off, non zero = on
    addOutput("arpeggio_mode", "Arpeggio Mode", WAY3, false); // -1 = down, 0 = up and down, 1 = up
    addOutput("arpeggio_range", "Arpeggio Range", WAY3, false); // -1 = 3, 0 = 2, 1 = 1
    addOutput("arpeggio_rate", "Arpeggio Rate", false);

    addOutput("lfo_rate", "LFO Rate", false);
    addOutput("lfo_delay", "LFO Delay", false);
    addOutput("lfo_mode", "LFO Mode", WAY2, false); // 0 = auto, non zero = manual

    addOutput("dco_lfo", "DCO LFO", false);
    addOutput("dco_pwm", "DCO PWM", false);
    addOutput("dco_pwm_mod", "DCO PWM Mode", WAY3, false); // -1 = env, 0 = manual, 1 = lfo

    addOutput("dco_pulse_switch", "Pulse", WAY2, false);
    addOutput("dco_saw_switch", "Sawtooth", WAY2, false);
    addOutput("dco_sub_switch", "DCO Sub Switch", WAY2, false);
    addOutput("dco_sub", "DCO Sub", false);
    addOutput("dco_noise", "Noise", false);

    addOutput("hpf_frq", "HPF Frequency", false);

    addOutput("vcf_frq", "VCF Frequency", false);
    addOutput("vcf_res", "VCF Resoncance", false);

    addOutput("vcf_env_invert", "VCF Envelope Invert", WAY2, false); // 0 = normal, non zero = inverted

    addOutput("vcf_env", "VCF Envelope", false);
    addOutput("vcf_lfo", "VCF LFO", false);
    addOutput("vcf_kbd", "VCF Keyboard", false);
    addOutput("vca_mode", "VCA Mode", WAY2, false); // 0 == env, 1 == gate
    addOutput("env_attack", "Attack", false);
    addOutput("env_decay", "Decay", false);
    addOutput("env_sustain", "Sustain", false);
    addOutput("env_release", "Release", false);
    addOutput("chorus_off_switch", "Chorus Off", WAY2, false);
    addOutput("chorus_I_switch", "Chorus I", WAY2, false);
    addOutput("chorus_II_switch", "Chorus II", WAY2, false);
    addOutput("panning", "Panning", false); //new and works only in stereo mode
    addOutput("stereo_switch", "Stereo Mode", WAY2, false);
	addOutput("unisono","UniSono",WAY2,false);
    addOutput("patch_change", false);
	

    for (int i = 0;i < voices;i++)
    {
        addOutput((String)"voice" + i + (String)"_pitch", false);
        addOutput((String)"voice" + i + (String)"_gate", false);
    }
}
