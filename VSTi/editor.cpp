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

    addSlider(260, 33, "arpeggio_rate");

    addSlider(281, 33, "lfo_rate");
    addSlider(299, 33, "lfo_delay");

    addSlider(346, 33, "dco_lfo");
    addSlider(364, 33, "dco_pwm");
    addSlider(460, 33, "dco_sub");
    addSlider(478, 33, "dco_noise");

    addSlider(502, 33, "hpf_frq");

    addSlider(526, 33, "vcf_frq");
    addSlider(543, 33, "vcf_res");
    addSlider(581, 33, "vcf_env");
    addSlider(600, 33, "vcf_lfo");
    addSlider(618, 33, "vcf_kbd");

    addSlider(689, 33, "env_attack");
    addSlider(707, 33, "env_decay");
    addSlider(726, 33, "env_sustain");
    addSlider(743, 33, "env_release");

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
    long ctlid = ccontrol->getTag();
    
    effect->setParameterAutomated (ctlid, ccontrol->getValue());
    ccontrol->update(canvas);
}

void
Editor::addSlider (int x, int y, char *outputName)
{   
    CBitmap *sliderBg = new CBitmap (IDB_SLIDER_BG);
    CBitmap *sliderPick = new CBitmap (IDB_SLIDER_PICK);

    int tag = control->getOutputNum(outputName);

    CRect size( 0, 0, sliderBg->getWidth(), sliderBg->getHeight() );

    size.offset(x, y);

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
