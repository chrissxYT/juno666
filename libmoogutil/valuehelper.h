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
	if (abs(step)<speed)
		{
			*orig = destination;
			return true;
		}
	if (step<0)*orig-=speed;
	else	   *orig+=speed;
	}
	return false;
}
#endif