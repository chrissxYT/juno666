#ifndef __inc_editor__
#define __inc_editor__

#include "vstgui.h"

#include <libmoog/moog.h>

class Editor: public AEffGUIEditor, public CControlListener
{
public:
	Editor(AudioEffect *effect, JunoControl *cntrl, MidiInput *midiinput);
	~Editor();

	void addKnob(int x, int y, char *outputName);
	void addButton(int x, int y, int bmp, char *outputName);
	void addSlider(int x, int y, char *outputName);
	void addTwoSwitch(int x, int y, char *outputName);
	void addThreeSwitch(int x, int y, char *outputName);
	void addKickButton(int x, int y, int bmp, char *outputName);

	void addKey(int x, int y, int key, int bmp);

protected:

	virtual long open(void *ptr);
	virtual void close();

	virtual void setParameter(long index, float value);
	virtual void valueChanged(CDrawContext *canvas, CControl *control);

	float getParameter(long index);

private:

	MidiInput *midiInput;

	JunoControl *control;

	CBitmap *bmpBackground;

	CControl **guiControl;

	CControl **guiKeyboard;
};

#endif
