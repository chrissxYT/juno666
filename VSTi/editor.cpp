#include "editor.h"

#include "../resource/resource.h"

#define MAX_KEYBOARD_KEYS 64

#define KEY_CONTROL_TAG_OFFSET 1024

Editor::Editor (AudioEffect* fx, JunoControl *cntrl, MidiInput *midiinput) : 
AEffGUIEditor(fx),
control(cntrl),
midiInput(midiinput)
{
    guiControl = new CControl *[control->getNumOutputs()];

    guiKeyboard = new CControl *[MAX_KEYBOARD_KEYS];
	int i;
    for(i=0; i<control->getNumOutputs(); i++)
        guiControl[i] = NULL;

    for(i=0; i<MAX_KEYBOARD_KEYS; i++)
        guiKeyboard[i] = NULL;

    bmpBackground = new CBitmap(IDB_BACKGROUND);
    rect.top = rect.left = 0;
    rect.bottom = (short)bmpBackground->getHeight ();
    rect.right =  (short)bmpBackground->getWidth ();
}

Editor::~Editor ()
{
    if (bmpBackground) bmpBackground->forget ();
}

void
Editor::close ()
{
    delete frame;
    frame = 0;
}

long
Editor::open (void* ptr)
{
    AEffGUIEditor::open(ptr);

    CRect size( 0, 0, bmpBackground->getWidth(), bmpBackground->getHeight() );

    frame = new CFrame (size, ptr, this);
    frame->setBackground (bmpBackground);


	/* UniSono */
	addButton(900,43,IDB_WHITE_BUTTON, "unisono");


    addKnob (80, 52, "master_tune");

    addSlider(22, 127, "bender_dco");
    addSlider(41, 127, "bender_vcf");
    addThreeSwitch(70, 142, "octave_transpose");
    addKickButton(75, 182, IDB_LFO_KICK, "lfo_trigger");

    addButton(125, 43, IDB_WHITE_BUTTON, "transpose_switch");
    addButton(152, 43, IDB_YELLOW_BUTTON, "hold_switch");

    /* Arpeggio */
    addButton(184, 43, IDB_ORANGE_BUTTON, "arpeggio_switch");
    addThreeSwitch(205, 48, "arpeggio_mode");
    addThreeSwitch(242, 48, "arpeggio_range");
    addSlider(260, 33, "arpeggio_rate");

    /* LFO */
    addSlider(281, 33, "lfo_rate");
    addSlider(299, 33, "lfo_delay");
    addTwoSwitch(314, 56, "lfo_mode");

    /* DCO */
    addSlider(346, 33, "dco_lfo");
    addSlider(364, 33, "dco_pwm");
    addThreeSwitch(379, 48, "dco_pwm_mod");
    addButton(407, 43, IDB_WHITE_BUTTON, "dco_pulse_switch");
    addButton(423, 43, IDB_YELLOW_BUTTON, "dco_saw_switch");
    addButton(439, 43, IDB_ORANGE_BUTTON, "dco_sub_switch");
    addSlider(460, 33, "dco_sub");
    addSlider(478, 33, "dco_noise");

    /* HPF */
    addSlider(502, 33, "hpf_frq");

    /* VCF */
    addSlider(526, 33, "vcf_frq");
    addSlider(543, 33, "vcf_res");
    addTwoSwitch(565, 56, "vcf_env_invert");
    addSlider(581, 33, "vcf_env");
    addSlider(600, 33, "vcf_lfo");
    addSlider(618, 33, "vcf_kbd");

    /* VCA */
    addTwoSwitch(640, 56, "vca_mode");
    addSlider(668, 33, "volume");

    /* ENV */
    addSlider(689, 33, "env_attack");
    addSlider(707, 33, "env_decay");
    addSlider(726, 33, "env_sustain");
    addSlider(743, 33, "env_release");

    /* Chorus */
    addKickButton(765, 52, IDB_WHITE_KBUTTON, "chorus_off_switch");
    addButton(781, 43, IDB_YELLOW_BUTTON, "chorus_I_switch");
    addButton(797, 43, IDB_ORANGE_BUTTON, "chorus_II_switch");

    /* Keyboard */
    addKey(130, 111,  0, IDB_C);
    addKey(152, 111,  2, IDB_D);
    addKey(174, 111,  4, IDB_E);
    addKey(196, 111,  5, IDB_F);
    addKey(218, 111,  7, IDB_G);
    addKey(240, 111,  9, IDB_A);
    addKey(262, 111, 11, IDB_B);

    addKey(144, 111,  1, IDB_HALF);
    addKey(171, 111,  3, IDB_HALF);
    addKey(210, 111,  6, IDB_HALF);
    addKey(234, 111,  8, IDB_HALF);
    addKey(259, 111, 10, IDB_HALF);

    addKey(284, 111, 12, IDB_C);
    addKey(306, 111, 14, IDB_D);
    addKey(328, 111, 16, IDB_E);
    addKey(350, 111, 17, IDB_F);
    addKey(372, 111, 19, IDB_G);
    addKey(394, 111, 21, IDB_A);
    addKey(416, 111, 23, IDB_B);

    addKey(298, 111, 13, IDB_HALF);
    addKey(325, 111, 14, IDB_HALF);
    addKey(364, 111, 18, IDB_HALF);
    addKey(388, 111, 20, IDB_HALF);
    addKey(413, 111, 22, IDB_HALF);

    addKey(438, 111, 24, IDB_C);
    addKey(460, 111, 26, IDB_D);
    addKey(482, 111, 28, IDB_E);
    addKey(504, 111, 29, IDB_F);
    addKey(526, 111, 31, IDB_G);
    addKey(548, 111, 33, IDB_A);
    addKey(570, 111, 35, IDB_B);

    addKey(452, 111, 25, IDB_HALF);
    addKey(479, 111, 27, IDB_HALF);
    addKey(518, 111, 30, IDB_HALF);
    addKey(542, 111, 32, IDB_HALF);
    addKey(567, 111, 34, IDB_HALF);

    addKey(592, 111, 36, IDB_C);
    addKey(614, 111, 38, IDB_D);
    addKey(636, 111, 40, IDB_E);
    addKey(658, 111, 41, IDB_F);
    addKey(680, 111, 43, IDB_G);
    addKey(702, 111, 45, IDB_A);
    addKey(724, 111, 47, IDB_B);

    addKey(606, 111, 37, IDB_HALF);
    addKey(633, 111, 39, IDB_HALF);
    addKey(672, 111, 42, IDB_HALF);
    addKey(696, 111, 44, IDB_HALF);
    addKey(721, 111, 46, IDB_HALF);

    addKey(746, 111, 48, IDB_C);
    addKey(768, 111, 50, IDB_D);
    addKey(790, 111, 52, IDB_E);
    addKey(812, 111, 53, IDB_F);
    addKey(834, 111, 55, IDB_G);
    addKey(856, 111, 57, IDB_A);
    addKey(878, 111, 59, IDB_B);

    addKey(760, 111, 49, IDB_HALF);
    addKey(787, 111, 51, IDB_HALF);
    addKey(826, 111, 54, IDB_HALF);
    addKey(850, 111, 56, IDB_HALF);
    addKey(875, 111, 58, IDB_HALF);

    addKey(900, 111, 60, IDB_LAST);

    for(int i=0; i<control->getNumOutputs(); i++)
    {
        setParameter(i, getParameter(i));
    }

    return true;    
}

void
Editor::setParameter (long index, float value)
{   
    if (frame)
    {
        if (index & KEY_CONTROL_TAG_OFFSET)
        {
            guiKeyboard[index-KEY_CONTROL_TAG_OFFSET]->setValue(value);
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
Editor::valueChanged (CDrawContext* canvas, CControl* ccontrol)
{   
    long tag = ccontrol->getTag();

    // hnadle gui keyboard
    if(tag & KEY_CONTROL_TAG_OFFSET)
    {
        if(ccontrol->getValue()==1)
        {
            midiInput->doNoteOn(0, (tag-KEY_CONTROL_TAG_OFFSET) + 36, 127);
        }
        else
        {
            midiInput->doNoteOff(0, (tag-KEY_CONTROL_TAG_OFFSET) + 36, 127);
        }

        ccontrol->update(canvas);
        return;
    }

    // handle chorus off switch
    if (tag == control->getOutputNum("chorus_off_switch"))
    {
        effect->setParameterAutomated (control->getOutputNum("chorus_I_switch"), 0);
        effect->setParameterAutomated (control->getOutputNum("chorus_II_switch"), 0);
    }

    // handle rest of the parameters
    effect->setParameterAutomated (tag, ccontrol->getValue());
    ccontrol->update(canvas);
}

void
Editor::addSlider (int x, int y, char *outputName)
{   
    CBitmap *sliderBg = new CBitmap(IDB_SLIDER_BG);
    CBitmap *sliderPick = new CBitmap(IDB_SLIDER_PICK);

    int tag = control->getOutputNum(outputName);

    CRect size( x, y, x + sliderBg->getWidth(), y + sliderBg->getHeight() );

    CPoint point(0, 0);

    guiControl[tag] = new CVerticalSlider( size, this, tag, 
                               size.top + 2, 
                               size.top + sliderBg->getHeight() - sliderPick->getHeight(), 
                               sliderPick, sliderBg, point, kBottom);
    point(1, 0);

    ((CVerticalSlider *)guiControl[tag])->setOffsetHandle(point);

    ((CVerticalSlider *)guiControl[tag])->setFreeClick(false);

    frame->addView(guiControl[tag]);

    sliderBg->forget();
    sliderPick->forget();
}

void
Editor::addButton (int x, int y, int bmp, char *outputName)
{   
    CBitmap *bitmap = new CBitmap(bmp);

    int tag = control->getOutputNum(outputName);

    CRect size(x, y, x + bitmap->getWidth(), y + bitmap->getHeight() / 2);

    guiControl[tag] = new COnOffButton(size, this, tag, bitmap);

    frame->addView(guiControl[tag]);

    bitmap->forget();
}

void
Editor::addTwoSwitch (int x, int y, char *outputName)
{   
    CBitmap *bitmap = new CBitmap(IDB_2_SWITCH);

    int tag = control->getOutputNum(outputName);

    CRect size(x, y, x + bitmap->getWidth(), y + bitmap->getHeight() / 2);

    CPoint point(0, 0);

    guiControl[tag] = new CVerticalSwitch(size, this, tag, 2, bitmap->getHeight() / 2, 2, bitmap, point);

    frame->addView(guiControl[tag]);

    bitmap->forget();
}

void
Editor::addThreeSwitch (int x, int y, char *outputName)
{   
    CBitmap *bitmap = new CBitmap(IDB_3_SWITCH);

    int tag = control->getOutputNum(outputName);

    CRect size(x, y, x + bitmap->getWidth(), y + bitmap->getHeight() / 3);

    CPoint point(0, 0);

    guiControl[tag] = new CVerticalSwitch(size, this, tag, 3, bitmap->getHeight() / 3, 3, bitmap, point);

    frame->addView(guiControl[tag]);

    bitmap->forget();
}

void
Editor::addKey (int x, int y, int key, int bmp)
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
Editor::addKnob (int x, int y, char *outputName)
{   
    CBitmap *bitmap = new CBitmap(IDB_KNOB);

    int tag = control->getOutputNum(outputName);

    CRect size(x, y, x + bitmap->getWidth(), y + bitmap->getHeight() / 100);

    CPoint point(0, 0);

    guiControl[tag] = new CAnimKnob(size, this, tag, 100, bitmap->getHeight() / 100, bitmap, point);

    frame->addView(guiControl[tag]);

    bitmap->forget();
}

void
Editor::addKickButton (int x, int y, int bmp, char *outputName)
{   
    CBitmap *bitmap = new CBitmap(bmp);

    int tag = control->getOutputNum(outputName);

    CRect size(x, y, x + bitmap->getWidth(), y + bitmap->getHeight() / 2);

    CPoint point(0, 0);

    guiControl[tag] = new CKickButton(size, this, tag, bitmap->getHeight() / 2, bitmap, point);

    frame->addView(guiControl[tag]);

    bitmap->forget();
}
