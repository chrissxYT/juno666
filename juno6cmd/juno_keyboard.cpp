/*
6.4. BrainSlayer

wrapper for keyboard functions, replaces GTK UI

*/


#include "juno_control.h"
#include "juno_keyboard.h"

extern Settings *settings;
extern JunoControl *junoControl;
extern MidiInput *midiInput;
extern JunoKeyboard *keyboard;


void JunoKeyboard_midiGateChanged(MoogObject *o, double data, long userData)
{
	((JunoKeyboard *)o)->midiGateChanged(userData, data);
}

void JunoKeyboard_gtkKeyPressed(int voice, int key, void *data)
{
	((JunoKeyboard *)data)->gtkKeyPressed(voice, key);
}

void JunoKeyboard_gtkKeyReleased(int voice, void *data)
{
	((JunoKeyboard *)data)->gtkKeyReleased(voice);
}

void JunoKeyboard_octaveTransposeChanged(MoogObject *o, double data, long)
{
	((JunoKeyboard *)o)->octaveTransposeChanged(data);
}

void JunoKeyboard_keyTransposeChanged(MoogObject *o, double data, long)
{
	((JunoKeyboard *)o)->keyTransposeChanged(data);
}

void JunoKeyboard_masterTuneChanged(MoogObject *o, double data, long)
{
	((JunoKeyboard *)o)->masterTuneChanged(data);
}

void JunoKeyboard_holdChanged(MoogObject *o, double data, long)
{
	((JunoKeyboard *)o)->holdChanged(data);
}

int initz=0;
JunoKeyboard::JunoKeyboard(int _numVoices)
{
	numVoices = _numVoices;
	octaveTranspose = -1;
	keyTransposePressed = 0;
	keyTranspose = 0;
	holdPressed = 0;
	savedGateInfo = new int[numVoices];
	pitchOutputs = new Output *[numVoices];
	gateOutputs = new Output *[numVoices];
	
	memset(savedGateInfo, 0, numVoices * sizeof(int));


	for (int i = 0;i < numVoices;i++)
	{
	
		String tmp1, tmp2;
		tmp1.sprintf("sig%d", i);
		tmp2.sprintf("voice%d_pitch", i);
		addInput(tmp2, NULL, 0, 0);
		PATCH(midiInput, tmp1, this, tmp2);
		pitchOutputs[i] = junoControl->getOutput(tmp2);
		tmp1.sprintf("amp%d", i);
		tmp2.sprintf("voice%d_gate", i);
		addInput(tmp2, JunoKeyboard_midiGateChanged, i, 1);
		PATCH(midiInput, tmp1, this, tmp2);
		gateOutputs[i] = junoControl->getOutput(tmp2);
	}

	masterTune = 0;
	addInput("octave_transpose", JunoKeyboard_octaveTransposeChanged, 0, 1);
	addInput("transpose_switch", JunoKeyboard_keyTransposeChanged, 0, 1);
	addInput("master_tune", JunoKeyboard_masterTuneChanged, 0, 1);
	addInput("hold_switch", JunoKeyboard_holdChanged, 0, 1);

	PATCH(junoControl, "octave_transpose", this, "octave_transpose");
	PATCH(junoControl, "transpose_switch", this, "transpose_switch");
	PATCH(junoControl, "master_tune", this, "master_tune");
	PATCH(junoControl, "hold_switch", this, "hold_switch");
	initz=1;
}

JunoKeyboard::~JunoKeyboard()
{
	delete[]savedGateInfo;
	delete[]pitchOutputs;
	delete[]gateOutputs;
}





void JunoKeyboard::midiGateChanged(int voiceNum, double data)
{
if (!initz)return;
	//printf("%f\n",inputs[voiceNum * 2].data);
	if (data > 0)
	{
		JunoKeyboard::gtkKeyPressed(voiceNum,pitch_to_note(Scheduler::nyquistFreq * *inputs[voiceNum * 2].data) - 36);

	}
	else
	{
		JunoKeyboard::gtkKeyReleased(voiceNum);
	}
	
}

void JunoKeyboard::gtkKeyPressed(unsigned int voice, unsigned int key)
{
	//printf("----------------key %u was pressed on voice %u\n", key, voice);

	if (keyTransposePressed)
	{
		transposeVoices(pow(2, ((double)(key % 12) - keyTranspose) / 12));
		keyTranspose = key % 12;
	}
	else
	{
		double val = CPS(midi_notes[key+36 - octaveTranspose * 12 + keyTranspose]) * pow(2, masterTune / 12);
		pitchOutputs[voice]->setData(val);
		gateOutputs[voice]->setData(1.0);

		savedGateInfo[voice] = 1;
	}
}

void JunoKeyboard::gtkKeyReleased(unsigned int voice)
{
	//printf("----------------voice %u was released\n", voice);

	if (!holdPressed)
	{
		
		if (gateOutputs[voice]==NULL)puts("bad condition");
		
		//gateOutputs[voice]->setData(0.0);
	}

	savedGateInfo[voice] = 0;

}


void JunoKeyboard::octaveTransposeChanged(double data)
{
	//printf("octave transpose changed to %f\n", data);

	int octaves = (int)octaveTranspose - (int)data;
	octaveTranspose = (int)data;

	transposeVoices(pow(2, (double)octaves));
}

void JunoKeyboard::keyTransposeChanged(double data)
{
	//printf("key transpose changed to %f\n", data);
	keyTransposePressed = (data) ? 1 : 0;
}


void JunoKeyboard::masterTuneChanged(double data)
{
	data = data * 2 - 1;
	double adjustment = pow(2, (data - masterTune) / 12.0);
	masterTune = data;

	//printf("master tune changed to %f adjustment=%f\n", data, adjustment);
	transposeVoices(adjustment);
}

void JunoKeyboard::holdChanged(double data)
{
	//printf("hold changed to %f\n", data);
	if (!(holdPressed = (data) ? 1 : 0))
	{
		for (int i = 0;i < numVoices;i++)
			if (!savedGateInfo[i])
				gateOutputs[i]->setData(0.0);
	}
}

void JunoKeyboard::transposeVoices(double tune)
{
	//printf( "TRANSPOSE BY %f\n", tune );
	for (int i = 0;i < numVoices;i++)
		pitchOutputs[i]->setData(pitchOutputs[i]->data * tune);
}