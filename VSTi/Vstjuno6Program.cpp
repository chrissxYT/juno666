#include <string.h>

#include "VstJuno6Program.h"

VstJuno6Program::VstJuno6Program ()
{
    bender_dco = 0;
    bender_vcf = 0;
    lfo_trigger = 0;
    volume = 0;
    octave_transpose = 0;
    arpeggio_switch = 0;
    arpeggio_mode = 0;
    arpeggio_range = 0;
    arpeggio_rate = 0;
    lfo_rate = 0;
    lfo_delay = 0;
    lfo_mode = 0;
    dco_lfo = 0;
    dco_pwm = 0;
    dco_pwm_mod = 0;
    dco_pulse_switch = 0;
    dco_saw_switch = 0;
    dco_sub_switch = 0;
    dco_sub = 0;
    dco_noise = 0;
    hpf_frq = 0;
    vcf_frq = 0;
    vcf_res = 0;
    vcf_env_invert = 0;
    vcf_env = 0;
    vcf_lfo = 0;
    vcf_kbd = 0;
    vca_mode = 0;
    env_attack = 0;
    env_decay = 0;
    env_sustain = 0;
    env_release = 0;
    chorus_I_switch = 0;
    chorus_II_switch = 0;

    strcpy (name, "Default");
}
