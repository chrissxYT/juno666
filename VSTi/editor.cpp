#include "editor.h"

#include "../resource/resource.h"

Editor::Editor (AudioEffect* fx, JunoControl *cntrl) : 
AEffGUIEditor(fx),
control(cntrl)
{
    guiControl = new CControl *[control->getNumOutputs()];

    for(int i=0; i<control->getNumOutputs(); i++)
        guiControl[i] = NULL;

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

    addSlider(22, 127, "bender_dco");
    addSlider(41, 127, "bender_vcf");
    addThreeSwitch(70, 140, "octave_transpose");

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
    addButton(765, 43, IDB_WHITE_BUTTON, "chorus_off_switch");
    addButton(781, 43, IDB_YELLOW_BUTTON, "chorus_I_switch");
    addButton(797, 43, IDB_ORANGE_BUTTON, "chorus_II_switch");

    for(int i=0; i<control->getNumOutputs(); i++)
    {
        setParameter(i, (float) * control->getOutput(i)->getData());
    }

    return true;    
}

void
Editor::setParameter (long index, float value)
{   
    if (frame)
    {
        if (guiControl[index])
        {
            guiControl[index]->setValue(value);

            postUpdate();
        }
    }
}

void
Editor::valueChanged (CDrawContext* canvas, CControl* ccontrol)
{   
    long tag = ccontrol->getTag();

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
