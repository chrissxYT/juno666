/*
this helper us used to do fine slide while control changes. it can be used for each parameter and uses a fixed
sliding precision
*/

#ifndef VALUEHELPER_H
#define VALUEHELPER_H

bool inline processParameter(double *orig, double destination,double speed)
{
	if (*orig != destination)
	{
	double step = destination - *orig;
	if (abs(step)<0.002)
		{
			*orig = destination;
			return true;
		}
	step *=speed;
	*orig+=step;
	return true;
	}
	return false;
}
bool inline processParameter(double *orig, double destination,double speed,double initialdiff)
{
	if (initialdiff==0.0){
		return false;
	}
	if (*orig != destination) //prevents divide by zeros too
	{
		double step = destination - *orig;
	if (abs(step)<0.002)
		{
			*orig = destination;
			return true;
		}
	step *=speed;
	step *=initialdiff;
	*orig+=step;
	return true;
	}
	return false;
}


#endif