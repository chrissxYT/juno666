#pragma once

#include <math.h>
#include "MoogObject.h"


#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif



class Phaser :
	public MoogObject
{

private:
	friend void phaser_frequency_changed(MoogObject *, double, long);
	friend void phaser_rate_changed(MoogObject *, double, long);
	friend void phaser_minrange_changed(MoogObject *, double, long);
	friend void phaser_maxrange_changed(MoogObject *, double, long);
	friend void phaser_feedback_changed(MoogObject *, double, long);
	friend void phaser_depth_changed(MoogObject *, double, long);
	
	double *inSig;
	double *m_dblMinRange;
	double *m_dblMaxRange;
	double *m_dblRate;
	double *m_dblFrequency;
	double *m_dblFeedback;
	double *m_dblDepth;

	Output *output;

public:
	Phaser(Scheduler *sched,ConnectionManager *conn);
	virtual ~Phaser(void);

	void setup(
		double frequency,
		double minrange,
		double maxrange,
		double rate,
		double feedback,
		double depth);
	void sampleGo();

	void connectTo(ConnectionInfo *info);
	void disconnectTo(ConnectionInfo *info);

	const char *getClassName()
	{
		return "Phaser";
	}

private:
	class AllpassDelay
	{
	public:
		AllpassDelay():_a1(0.f), _zm1(0.f)
	{
	}

	void Delay(double delay)
	{ //sample delay time
		_a1 = (1.0 - delay) / (1.0 + delay);
	}

	double Update(double inSamp)
	{
		double y = inSamp * -_a1 + _zm1;
		_zm1 = y * _a1 + inSamp;

		return y;
	}
private:
	double _a1, _zm1;
};

AllpassDelay _alps[6];


double _dmin, _dmax; //range
double _fb; //feedback
double _lfoPhase;
double _lfoInc;
double _depth;

double _zm1;
};
