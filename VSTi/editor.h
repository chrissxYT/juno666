#ifndef __inc_editor__
#define __inc_editor__

#include "vstgui.h"

#include <libmoog/moog.h>

class Editor: public AEffGUIEditor, public CControlListener, public MoogObject
{
public:
	Editor(Scheduler *sched,ConnectionManager *conn,AudioEffect *effect, Control *cntrl, MidiInput *midiinput);
	~Editor();

	const char *getClassName()
	{
		return "JunoEditor";
	}

	void addBender(int x, int y, int bmp, char *outputname);
	void addBitmap(int x, int y, int bmp, void *ptr); //no control yet
	void addKnob(int x, int y, char *outputName);
	void addButton(int x, int y, int bmp, char *outputName);
	void changeControl(long tag,double value);
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
	CBitmap *bmpBackground;
	Control *control;

	CControl **guiControl;

	CControl **guiKeyboard;
};

#endif
