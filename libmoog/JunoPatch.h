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
 * $Revision: 1.5 $$Date: 2004/09/11 00:12:34 $
 */
#ifndef JUNO_PATCH_H
#define JUNO_PATCH_H

#define PATCH_NAME_LEN 40

typedef struct
{
	int version;
	int used;
	float bender_dco;
	float bender_vcf;
	float lfo_trigger;
	float volume;
	float octave_transpose;
	float arpeggio_switch;
	float arpeggio_mode;
	float arpeggio_range;
	float arpeggio_rate;
	float lfo_rate;
	float lfo_delay;
	float lfo_mode;
	float dco_lfo;
	float dco_pwm;
	float dco_pwm_mod;
	float dco_pulse_switch;
	float dco_saw_switch;
	float dco_sub_switch;
	float dco_sub;
	float dco_noise;
	float hpf_frq;
	float vcf_frq;
	float vcf_res;
	float vcf_env_invert;
	float vcf_env;
	float vcf_lfo;
	float vcf_kbd;
	float vca_mode;
	float env_attack;
	float env_decay;
	float env_sustain;
	float env_release;
	float chorus_I_switch;
	float chorus_II_switch;
	float panning;
	float unisono;
	float phaserrate;
	float phaserfeedback;
	char name[PATCH_NAME_LEN];
} juno_patch;

#define NUM_PATCHES 128
#ifdef __cplusplus
extern "C"
{
#endif
juno_patch *juno_patchset_new();
void juno_patchset_delete();
int save_patches(const char *filename, juno_patch *patches);
int load_patches(const char *filename, juno_patch *patches);
#ifdef __cplusplus
}
#endif
#endif /* JUNO_PATCH_H */



