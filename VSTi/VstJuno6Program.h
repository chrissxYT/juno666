#ifndef __VstJuno6Program_H__
#define __VstJuno6Program_H__

class VstJuno6Program
{
friend class VstJuno6;
public:
    VstJuno6Program ();
    ~VstJuno6Program () {}

private:
    double bender_dco;
    double bender_vcf;
    double lfo_trigger;
    double volume;
    double octave_transpose;
    double arpeggio_switch;
    double arpeggio_mode;
    double arpeggio_range;
    double arpeggio_rate;
    double lfo_rate;
    double lfo_delay;
    double lfo_mode;
    double dco_lfo;
    double dco_pwm;
    double dco_pwm_mod;
    double dco_pulse_switch;
    double dco_saw_switch;
    double dco_sub_switch;
    double dco_sub;
    double dco_noise;
    double hpf_frq;
    double vcf_frq;
    double vcf_res;
    double vcf_env_invert;
    double vcf_env;
    double vcf_lfo;
    double vcf_kbd;
    double vca_mode;
    double env_attack;
    double env_decay;
    double env_sustain;
    double env_release;
    double chorus_I_switch;
    double chorus_II_switch;
    char name[24];
};

#endif /* __VstJuno6Program_H__ */
