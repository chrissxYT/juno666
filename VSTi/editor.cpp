#include "editor.h"

#include "../resource/resource.h"
#include "junoui.h"

#define MAX_KEYBOARD_KEYS 64

#define KEY_CONTROL_TAG_OFFSET 1024

Editor::Editor(Scheduler *sched, ConnectionManager *conn,AudioEffect *fx, JunoControl *cntrl, MidiInput *midiinput): 
    MoogObject(sched,conn),
    AEffGUIEditor(fx),
	control(cntrl),
	midiInput(midiinput)
{
	guiControl = new CControl *[control->getNumOutputs()];

	guiKeyboard = new CControl *[MAX_KEYBOARD_KEYS];
	int i;
	for (i = 0;i < control->getNumOutputs();i++)
		guiControl[i] = NULL;

	for (i = 0;i < MAX_KEYBOARD_KEYS;i++)
		guiKeyboard[i] = NULL;

}

Editor::~Editor()
{
	if (bmpBackground)
		bmpBackground->forget();
}

void
Editor::close()
{
	delete frame;
	frame = 0;
}

long
Editor::open(void *ptr)
{
	AEffGUIEditor::open(ptr);


	int idx = 0;

	while (true)
	{
		UITYPES type = juno6ui[idx++];
		if (type.controltype == 0)
			break;
		switch (type.controltype)
		{
			case UIBITMAP:
				addBitmap(type.posx, type.posy, type.resource_id, ptr);
				break;
			case UIBUTTON:
				addButton(type.posx, type.posy, type.resource_id, type.control_name);
				break;
			case UIKNOB:
				addKnob(type.posx, type.posy, type.control_name);
				break;
			case UISLIDER:
				addSlider(type.posx, type.posy, type.control_name);
				break;
			case UITHREESWITCH:
				addThreeSwitch(type.posx, type.posy, type.control_name);
				break;
			case UIKICKBUTTON:
				addKickButton(type.posx, type.posy, type.resource_id, type.control_name);
				break;
			case UITWOSWITCH:
				addTwoSwitch(type.posx, type.posy, type.control_name);
				break;
			case UIKEY:
				addKey(type.posx, type.posy, type.controlidx, type.resource_id);
				break;
		}
	}






	for (int i = 0;i < control->getNumOutputs();i++)
	{
		setParameter(i, getParameter(i));
	}

	return true;
}

void
Editor::setParameter(long index, float value)
{
	if (frame)
	{
		if (index & KEY_CONTROL_TAG_OFFSET)
		{
			guiKeyboard[index - KEY_CONTROL_TAG_OFFSET]->setValue(value);
			postUpdate();
			return;
		}

		if (guiControl[index])
		{
			guiControl[index]->setValue(value);
			postUpdate();
		}
	}
}


// FIXME: I allready exist in VstJuno6
float
Editor::getParameter(long index)
{
	if (control)
	{
		float value = (float) * control->getOutput(index)->getData();
		switch (control->getOutput(index)->getType())
		{
			case WAY2:
				return value;
				break;
			case WAY3:
				value += 1;
				value /= 2;
				return value;
				break;
			default:
				return value;

		}
		return value;
	}

	return 0;
}

void
Editor::valueChanged(CDrawContext *canvas, CControl *ccontrol)
{
	long tag = ccontrol->getTag();

	// hnadle gui keyboard
	if (tag & KEY_CONTROL_TAG_OFFSET)
	{
		if (ccontrol->getValue() == 1)
		{
			midiInput->doNoteOn(0, (tag - KEY_CONTROL_TAG_OFFSET) + 36, 127);
		}
		else
		{
			midiInput->doNoteOff(0, (tag - KEY_CONTROL_TAG_OFFSET) + 36, 127);
		}

		ccontrol->update(canvas);
		return;
	}

	// handle chorus off switch
	if (tag == control->getOutputNum("chorus_off_switch"))
	{
		effect->setParameterAutomated(control->getOutputNum("chorus_I_switch"), 0);
		effect->setParameterAutomated(control->getOutputNum("chorus_II_switch"), 0);
	}

	// handle rest of the parameters
	effect->setParameterAutomated(tag, ccontrol->getValue());
	ccontrol->update(canvas);
}


void Editor::addBitmap(int x, int y, int bmp, void *ptr)
{
	bmpBackground = new CBitmap(bmp);
	rect.top = y;
	rect.left = x;
	rect.bottom = (short)bmpBackground->getHeight();
	rect.right = (short)bmpBackground->getWidth();
	CRect size(0, 0, bmpBackground->getWidth(), bmpBackground->getHeight());
	frame = new CFrame(size, ptr, this);
	frame->setBackground(bmpBackground);
}
void controlChanged(MoogObject *o, double data, long userdata)
{
	((Editor *)o)->changeControl(userdata,data);
}

void Editor::changeControl(long tag,double value)
{
		switch (control->getOutput(tag)->getType())
		{
			case WAY2:
				break;
			case WAY3:
				value += 1;
				value /= 2;
				break;
			default:
				break;

		}
	guiControl[tag]->setValue(value);
}

void
Editor::addSlider(int x, int y, char *outputName)
{
	CBitmap *sliderBg = new CBitmap(IDB_SLIDER_BG);
	CBitmap *sliderPick = new CBitmap(IDB_SLIDER_PICK);

	int tag = control->getOutputNum(outputName);
	

	CRect size(x, y, x + sliderBg->getWidth(), y + sliderBg->getHeight());

	CPoint point(0, 0);
	
	guiControl[tag] = new CVerticalSlider(size, this, tag,
		size.top + 2,
		size.top + sliderBg->getHeight() - sliderPick->getHeight(),
		sliderPick, sliderBg, point, kBottom);
	point(1, 0);

	((CVerticalSlider *)guiControl[tag])->setOffsetHandle(point);

	((CVerticalSlider *)guiControl[tag])->setFreeClick(false);

	frame->addView(guiControl[tag]);
	addInput(outputName,controlChanged,tag,0);
	PATCH(control,outputName,this,outputName);


	sliderBg->forget();
	sliderPick->forget();
}







void
Editor::addButton(int x, int y, int bmp, char *outputName)
{
	CBitmap *bitmap = new CBitmap(bmp);

	int tag = control->getOutputNum(outputName);

	CRect size(x, y, x + bitmap->getWidth(), y + bitmap->getHeight() / 2);

	guiControl[tag] = new COnOffButton(size, this, tag, bitmap);

	frame->addView(guiControl[tag]);
	addInput(outputName,controlChanged,tag,0);
	PATCH(control,outputName,this,outputName);

	bitmap->forget();
}

void
Editor::addTwoSwitch(int x, int y, char *outputName)
{
	CBitmap *bitmap = new CBitmap(IDB_2_SWITCH);

	int tag = control->getOutputNum(outputName);

	CRect size(x, y, x + bitmap->getWidth(), y + bitmap->getHeight() / 2);

	CPoint point(0, 0);

	guiControl[tag] = new CVerticalSwitch(size, this, tag, 2, bitmap->getHeight() / 2, 2, bitmap, point);

	frame->addView(guiControl[tag]);
	addInput(outputName,controlChanged,tag,0);
	PATCH(control,outputName,this,outputName);

	bitmap->forget();
}

void
Editor::addThreeSwitch(int x, int y, char *outputName)
{
	CBitmap *bitmap = new CBitmap(IDB_3_SWITCH);

	int tag = control->getOutputNum(outputName);

	CRect size(x, y, x + bitmap->getWidth(), y + bitmap->getHeight() / 3);

	CPoint point(0, 0);

	guiControl[tag] = new CVerticalSwitch(size, this, tag, 3, bitmap->getHeight() / 3, 3, bitmap, point);

	frame->addView(guiControl[tag]);
	addInput(outputName,controlChanged,tag,0);
	PATCH(control,outputName,this,outputName);

	bitmap->forget();
}

void
Editor::addKey(int x, int y, int key, int bmp)
{
	if (key > MAX_KEYBOARD_KEYS)
		return;

	CBitmap *bitmap = new CBitmap(bmp);

	 bitmap->setTransparentColor(kWhiteCColor);

	CRect size(x, y, x + bitmap->getWidth(), y + bitmap->getHeight() / 2);

	CPoint point(0, 0);

	guiKeyboard[key] = new CKickButton(size, this, key | KEY_CONTROL_TAG_OFFSET, bitmap->getHeight() / 2, bitmap, point);

	guiKeyboard[key]->setTransparency(true);

	frame->addView(guiKeyboard[key]);

	bitmap->forget();
}

void
Editor::addKnob(int x, int y, char *outputName)
{
	CBitmap *bitmap = new CBitmap(IDB_KNOB);

	int tag = control->getOutputNum(outputName);

	CRect size(x, y, x + bitmap->getWidth(), y + bitmap->getHeight() / 100);

	CPoint point(0, 0);

	guiControl[tag] = new CAnimKnob(size, this, tag, 100, bitmap->getHeight() / 100, bitmap, point);

	frame->addView(guiControl[tag]);
	addInput(outputName,controlChanged,tag,0);
	PATCH(control,outputName,this,outputName);

	bitmap->forget();
}

void
Editor::addKickButton(int x, int y, int bmp, char *outputName)
{
	CBitmap *bitmap = new CBitmap(bmp);

	int tag = control->getOutputNum(outputName);

	CRect size(x, y, x + bitmap->getWidth(), y + bitmap->getHeight() / 2);

	CPoint point(0, 0);

	guiControl[tag] = new CKickButton(size, this, tag, bitmap->getHeight() / 2, bitmap, point);

	frame->addView(guiControl[tag]);
	addInput(outputName,controlChanged,tag,0);
	PATCH(control,outputName,this,outputName);

	bitmap->forget();
}
