#include "Phaser.h"

void
phaser_frequency_changed(MoogObject *o, double data, long)
{
	//Phaser *p = (Phaser*)o;
	//p->setup(data,*p->m_dblMinRange,*p->m_dblMaxRange,*p->m_dblRate,*p->m_dblFeedback,*p->m_dblDepth);

}
void
phaser_minrange_changed(MoogObject *o, double data, long)
{
	Phaser *p = (Phaser*)o;
	//p->setup(p->m_dblFrequency,data,p->m_dblMaxRange,p->m_dblRate,p->m_dblFeedback,p->m_dblDepth);

}
void
phaser_maxrange_changed(MoogObject *o, double data, long)
{
	Phaser *p = (Phaser*)o;
	//p->setup(p->m_dblFrequency,p->m_dblMinRange,data,p->m_dblRate,p->m_dblFeedback,p->m_dblDepth);

}
void
phaser_rate_changed(MoogObject *o, double data, long)
{
	Phaser *p = (Phaser*)o;
	p->setup(p->m_dblFrequency,p->m_dblMinRange,p->m_dblMaxRange,data,p->m_dblFeedback,p->m_dblDepth);

}
void
phaser_feedback_changed(MoogObject *o, double data, long)
{
	Phaser *p = (Phaser*)o;
	p->setup(p->m_dblFrequency,p->m_dblMinRange,p->m_dblMaxRange,p->m_dblRate,data,p->m_dblDepth);
}
void
phaser_depth_changed(MoogObject *o, double data, long)
{
	Phaser *p = (Phaser*)o;
	//p->setup(p->m_dblFrequency,p->m_dblMinRange,p->m_dblMaxRange,p->m_dblRate,p->m_dblFeedback,data);
}





Phaser::Phaser(MoogObject *inputobject, const char *inputname) : MoogObject(inputobject->schedule,inputobject->getConnectionManager())
{
	
	//signal routing
	addInput("sig",NULL,0,0);
	output = addOutput("sig",true);
	//parameter
	addInput("frequency",phaser_frequency_changed,0,1);
	//addInput("minrange",phaser_minrange_changed,0,1);
	//addInput("maxrange",phaser_maxrange_changed,0,1);
	addInput("rate",phaser_rate_changed,0,1);
	addInput("feedback",phaser_feedback_changed,0,1);
	//addInput("depth",phaser_depth_changed,0,1);
	setup(44100.0,440.0,1600.0,0.5,0.7,1.0);
	PATCH(inputobject,inputname,this,"sig");
	schedule->scheduleSampleRate(this, true);
}

Phaser::~Phaser(void)
{

}

void Phaser::connectTo(ConnectionInfo *info)
{
	MoogObject::connectTo(info);
	inSig = inputs[0].data;
	/*m_dblFrequency = inputs[1].data;
	m_dblMinRange = inputs[2].data;
	m_dblMaxRange = inputs[3].data;
	m_dblRate = inputs[4].data;
	m_dblFeedback = inputs[5].data;
	m_dblDepth = inputs[6].data;*/
	//setup(*m_dblFrequency,*m_dblMinRange,*m_dblMaxRange,*m_dblRate,*m_dblFeedback,*m_dblDepth);
}

void Phaser::disconnectTo(ConnectionInfo *info)
{
	MoogObject::disconnectTo(info);
	inSig = inputs[0].data;
	/*m_dblFrequency = inputs[1].data;
	m_dblMinRange = inputs[2].data;
	m_dblMaxRange = inputs[3].data;
	m_dblRate = inputs[4].data;
	m_dblFeedback = inputs[5].data;
	m_dblDepth = inputs[6].data;*/
	//setup(*m_dblFrequency,*m_dblMinRange,*m_dblMaxRange,*m_dblRate,*m_dblFeedback,*m_dblDepth);
}

void Phaser::setup(double frequency,double minrange,double maxrange,double rate,double feedback,double depth)
{
		m_dblFrequency = frequency;
		m_dblMinRange = minrange;
		m_dblMaxRange = maxrange;
		m_dblRate = rate;
		m_dblFeedback = feedback;
		m_dblDepth = depth;

		_dmin = minrange / (frequency / 2.f);
		_dmax = maxrange / (frequency / 2.f);
		_lfoInc = 2.f * M_PI * (rate / frequency);
		_fb = feedback;
		_depth = depth;
		/*_lfoPhase = 0.0;
		_zm1 = 0.0;
		for (int i = 0;i < 6;i++)
		{
		_alps[i]._a1 = 0.0;
		_alps[i]._zm1 = 0.0;
		}*/
}


void Phaser::sampleGo()
{
	double *input = inSig;
	double d = _dmin + (_dmax - _dmin) * ((sin(_lfoPhase) + 1.0) / 2.0);

	_lfoPhase += _lfoInc;
	if (_lfoPhase >= M_PI * 2.0)
		_lfoPhase -= M_PI * 2.0;

	//update filter coeffs
	for (int i = 0;i < 6;i++)
	{
		_alps[i].Delay(d);
	}

			//calculate output
	output->data = _zm1 = _alps[0].Update(_alps[1].Update(_alps[2].Update(_alps[3].Update(_alps[4].Update(_alps[5].Update(*inSig + _zm1 * _fb))))));
}