#ifndef JUNOUI_H
#define JUNOUI_H

#include "UITypes.h"



UITYPES juno6ui[] =
{
	{UIBITMAP, 0, 0, 0, IDB_BACKGROUND},
	{UIBUTTON, 900, 43, 0, IDB_WHITE_BUTTON, "unisono"},
	{UIBUTTON, 884,43,0,IDB_WHITE_BUTTON, "stereo_switch"},
	{UIKNOB, 80, 52, 0, 0, "master_tune"},
	{UISLIDER, 22, 127, 0, 0, "bender_dco"},
	{UISLIDER, 41, 127, 0, 0, "bender_vcf"},
	{UISLIDER, 60, 127, 0, 0, "bender"},
	{UITHREESWITCH, 70, 142, 0, 0, "octave_transpose"},
	{UIKICKBUTTON, 75, 182, 0, IDB_LFO_KICK, "lfo_trigger"},
	{UIBUTTON, 125, 43, 0, IDB_WHITE_BUTTON, "transpose_switch"},
	{UIBUTTON, 152, 43, 0, IDB_YELLOW_BUTTON, "hold_switch"},
	{UIBUTTON, 184, 43, 0, IDB_ORANGE_BUTTON, "arpeggio_switch"}, //arpeggio section
	{UITHREESWITCH, 205, 48, 0, 0, "arpeggio_mode"},
	{UITHREESWITCH, 242, 48, 0, 0, "arpeggio_range"},
	{UISLIDER, 260, 33, 0, 0, "arpeggio_rate"},
	{UISLIDER, 281, 33, 0, 0, "lfo_rate"}, //LFO Section
	{UISLIDER, 299, 33, 0, 0, "lfo_delay"},
	{UITWOSWITCH, 314, 56, 0, 0, "lfo_mode"},
	{UISLIDER, 346, 33, 0, 0, "dco_lfo"}, //DCO
	{UISLIDER, 364, 33, 0, 0, "dco_pwm"},
	{UITHREESWITCH, 379, 48, 0, 0, "dco_pwm_mod"},
	{UIBUTTON, 407, 43, 0, IDB_WHITE_BUTTON, "dco_pulse_switch"},
	{UIBUTTON, 423, 43, 0, IDB_YELLOW_BUTTON, "dco_saw_switch"},
	{UIBUTTON, 439, 43, 0, IDB_ORANGE_BUTTON, "dco_sub_switch"},
	{UISLIDER, 460, 33, 0, 0, "dco_sub"},
	{UISLIDER, 478, 33, 0, 0, "dco_noise"},
	{UISLIDER, 502, 33, 0, 0, "hpf_frq"}, //HPF
	{UISLIDER, 526, 33, 0, 0, "vcf_frq"}, //VCF
	{UISLIDER, 543, 33, 0, 0, "vcf_res"},
	{UITWOSWITCH, 565, 56, 0, 0, "vcf_env_invert"},
	{UISLIDER, 581, 33, 0, 0, "vcf_env"},
	{UISLIDER, 600, 33, 0, 0, "vcf_lfo"},
	{UISLIDER, 618, 33, 0, 0, "vcf_kbd"},
	{UITWOSWITCH, 640, 56, 0, 0, "vca_mode"}, //VCA
	{UISLIDER, 668, 33, 0, 0, "volume"},
	{UISLIDER, 689, 33, 0, 0, "env_attack"}, //ENV
	{UISLIDER, 707, 33, 0, 0, "env_decay"},
	{UISLIDER, 726, 33, 0, 0, "env_sustain"},
	{UISLIDER, 743, 33, 0, 0, "env_release"},
	{UIKICKBUTTON, 765, 52, 0, IDB_WHITE_KBUTTON, "chorus_off_switch"}, //Chorus
	{UIBUTTON, 781, 43, 0, IDB_YELLOW_BUTTON, "chorus_I_switch"},
	{UIBUTTON, 797, 43, 0, IDB_ORANGE_BUTTON, "chorus_II_switch"},
	{UIKEY, 130, 111, 0, IDB_C}, //Keys
	{UIKEY, 152, 111, 2, IDB_D},
	{UIKEY, 174, 111, 4, IDB_E},
	{UIKEY, 196, 111, 5, IDB_F},
	{UIKEY, 218, 111, 7, IDB_G},
	{UIKEY, 240, 111, 9, IDB_A},
	{UIKEY, 262, 111, 11, IDB_B},

	{UIKEY, 144, 111, 1, IDB_HALF},
	{UIKEY, 171, 111, 3, IDB_HALF},
	{UIKEY, 210, 111, 6, IDB_HALF},
	{UIKEY, 234, 111, 8, IDB_HALF},
	{UIKEY, 259, 111, 10, IDB_HALF},

	{UIKEY, 284, 111, 12, IDB_C},
	{UIKEY, 306, 111, 14, IDB_D},
	{UIKEY, 328, 111, 16, IDB_E},
	{UIKEY, 350, 111, 17, IDB_F},
	{UIKEY, 372, 111, 19, IDB_G},
	{UIKEY, 394, 111, 21, IDB_A},
	{UIKEY, 416, 111, 23, IDB_B},

	{UIKEY, 298, 111, 13, IDB_HALF},
	{UIKEY, 325, 111, 14, IDB_HALF},
	{UIKEY, 364, 111, 18, IDB_HALF},
	{UIKEY, 388, 111, 20, IDB_HALF},
	{UIKEY, 413, 111, 22, IDB_HALF},

	{UIKEY, 438, 111, 24, IDB_C},
	{UIKEY, 460, 111, 26, IDB_D},
	{UIKEY, 482, 111, 28, IDB_E},
	{UIKEY, 504, 111, 29, IDB_F},
	{UIKEY, 526, 111, 31, IDB_G},
	{UIKEY, 548, 111, 33, IDB_A},
	{UIKEY, 570, 111, 35, IDB_B},

	{UIKEY, 452, 111, 25, IDB_HALF},
	{UIKEY, 479, 111, 27, IDB_HALF},
	{UIKEY, 518, 111, 30, IDB_HALF},
	{UIKEY, 542, 111, 32, IDB_HALF},
	{UIKEY, 567, 111, 34, IDB_HALF},

	{UIKEY, 592, 111, 36, IDB_C},
	{UIKEY, 614, 111, 38, IDB_D},
	{UIKEY, 636, 111, 40, IDB_E},
	{UIKEY, 658, 111, 41, IDB_F},
	{UIKEY, 680, 111, 43, IDB_G},
	{UIKEY, 702, 111, 45, IDB_A},
	{UIKEY, 724, 111, 47, IDB_B},

	{UIKEY, 606, 111, 37, IDB_HALF},
	{UIKEY, 633, 111, 39, IDB_HALF},
	{UIKEY, 672, 111, 42, IDB_HALF},
	{UIKEY, 696, 111, 44, IDB_HALF},
	{UIKEY, 721, 111, 46, IDB_HALF},

	{UIKEY, 746, 111, 48, IDB_C},
	{UIKEY, 768, 111, 50, IDB_D},
	{UIKEY, 790, 111, 52, IDB_E},
	{UIKEY, 812, 111, 53, IDB_F},
	{UIKEY, 834, 111, 55, IDB_G},
	{UIKEY, 856, 111, 57, IDB_A},
	{UIKEY, 878, 111, 59, IDB_B},

	{UIKEY, 760, 111, 49, IDB_HALF},
	{UIKEY, 787, 111, 51, IDB_HALF},
	{UIKEY, 826, 111, 54, IDB_HALF},
	{UIKEY, 850, 111, 56, IDB_HALF},
	{UIKEY, 875, 111, 58, IDB_HALF},

	{UIKEY, 900, 111, 60, IDB_LAST},
	{0, 0, 0, 0, 0, NULL} //Terminator
};

#endif
