/*
Juno 106 Text Sysex to vstJuno Patch6 converter by BrainSlayer
changelog:
17.1. initial version - BrainSlayer
13.4. patch names added - Strepto
14.4. octave transpose support - BrainSlayer


this converter can be compiled under various standard c compilers like
Borland C, GNU C, MSVC.
please enable extended floating point support in MSVC, since the original setting
has a precision leak.

special thanks goes to Edy Hinzen for the good Sysex description and the various tools on 
http://www.hinzen.de/midi/juno-106

*/

#include <libmoog/JunoPatch.h>
#include <stdio.h>
#include <errno.h>
unsigned char desc[128];

unsigned char get3hex(FILE *in)
{
	int value1, value2;
	value1 = getc(in);
	if (value1 > ('0' - 1) && value1 < ('9' + 1))
		value1 -= '0';
	else
		value1 = value1 - 'A' + 10;
	value2 = getc(in);
	if (value2 > ('0' - 1) && value2 < ('9' + 1))
		value2 -= '0';
	else
		value2 = value2 - 'A' + 10;
	getc(in);
	return ((value1 << 4) + value2);
}

int loadPatFile(char *name, juno_patch *patch)
{
	FILE *in;
	int i;

	double lfo_rate;
	double lfo_delay;
	double dco_lfo;
	double dco_pwm;
	double dco_noise;
	double vcf_frq;
	double vcf_res;
	double vcf_env;
	double vcf_lfo;
	double vcf_kbd;
	double vca_level; //what?
	double env_attack;
	double env_decay;
	double env_sustain;
	double env_release;
	double dco_sub;
	double octave_transpose;
	unsigned char sw1;
	unsigned char sw2;

	in = fopen(name, "rb");
	if (in == NULL)
		return -1;

	for (i = 0;i < NUM_PATCHES;i++)
	{
		char *name = patch[i].name;
		char c = 0;
		int len = 0;

		patch[i].version = 1;
		patch[i].used = 1;

		lfo_rate = get3hex(in);
		lfo_delay = get3hex(in);
		dco_lfo = get3hex(in);
		dco_pwm = get3hex(in);
		dco_noise = get3hex(in);
		vcf_frq = get3hex(in);
		vcf_res = get3hex(in);
		vcf_env = get3hex(in);
		vcf_lfo = get3hex(in);
		vcf_kbd = get3hex(in);
		vca_level = get3hex(in);
		env_attack = get3hex(in);
		env_decay = get3hex(in);
		env_sustain = get3hex(in);
		env_release = get3hex(in);
		dco_sub = get3hex(in);
		sw1 = get3hex(in);
		sw2 = get3hex(in);
		printf("converting %d\n", i);

//get switches flag 1
		octave_transpose = 0;
		/*
		these switches are handled exclusive
		*/
		if (sw1 & 1) // octave up ('4)
			octave_transpose = -1;
		if (sw1 & 2) //octave normal ('8)
			octave_transpose = 0;
		if (sw1 & 4) //octave down ('16)
			octave_transpose = 1;

		if (i == 79)
			printf(" sw1 %d\n", sw1);
		if (sw1 & 8)
			patch[i].dco_pulse_switch = 1;
		if (sw1 & 16)
			patch[i].dco_saw_switch = 1;

		switch ((sw1 >> 5) & 3)
		{
			case 0:
				patch[i].chorus_II_switch = 1;
				break;
			case 1:
				break;
			default: //should be 2
				patch[i].chorus_I_switch = 1;
		}

//get switches flag 2
// -1 = env, 0 = man 1 = lfo
		if (sw2 & 1)
			patch[i].dco_pwm_mod = 1; //MAN
		else
			patch[i].dco_pwm_mod = 0; //LFO
		if (sw2 & 4)
			patch[i].vca_mode = 1; //GATE
		else
			patch[i].vca_mode = 0; //ENV
		if (sw2 & 2)
			patch[i].vcf_env_invert = 1; //polarity inverted
		else
			patch[i].vcf_env_invert = 0;
		patch[i].hpf_frq = ((double)((int)(sw2 >> 3) & 3)) / 8;
//controls

		patch[i].lfo_rate = (double)lfo_rate / 127;
		patch[i].lfo_delay = (double)lfo_delay / 127;
		patch[i].dco_lfo = (double)dco_lfo / 127;
		patch[i].dco_pwm = (double)dco_pwm / 127;
		patch[i].dco_noise = (double)dco_noise / 127;
		patch[i].vcf_frq = (double)vcf_frq / 127;
		patch[i].vcf_res = (double)vcf_res / 127;
		patch[i].vcf_env = (double)vcf_env / 127;
		patch[i].vcf_lfo = (double)vcf_lfo / 127;
		patch[i].vcf_kbd = (double)vcf_kbd / 127;
		patch[i].volume = (double)vca_level / 127;
		patch[i].env_attack = (double)env_attack / 127;
		patch[i].env_decay = (double)env_decay / 127;
		patch[i].env_sustain = (double)env_sustain / 127;
		patch[i].env_release = (double)env_release / 127;
		patch[i].dco_sub = (double)dco_sub / 127;
		patch[i].octave_transpose = octave_transpose;
//if an dco sub value is set, the dco switch will automaticly
//enabled. roland missed to implement the switch in the sysex chart
		if (dco_sub > 0)
			patch[i].dco_sub_switch = 1;

		c = getc(in);

//read the name of the patch till the end of the line
		while (c != 0xa)
		{
			if (len < PATCH_NAME_LEN && c != 0xd)
			{
				*name++ = c;
			}
			c = getc(in);
			len++;
		}
	}
}

int
main(int argc, char **argv)
{
	juno_patch *patches;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s 'patfile' 'patchfile'\n", argv[0]);
		return 1;
	}

	patches = juno_patchset_new();
	loadPatFile(argv[1], patches);

	save_patches(argv[2], patches);
	juno_patchset_delete(patches);

	return 0;
}
