/*
 * Copyright(c) 2000 UltraMaster Group
 *
 * License to use UltraMaster Juno-6 is provided free of charge subject to the
 * following restrictions:
 *
 * 1.) This license is for your personal use only.
 *
 * 2.) No portion of this software may be redistributed by you to any other
 *     person in any form.
 *
 * 3.) You may not sell UltraMaster Juno-6 to any person.
 *
 * 4.) UltraMaster Juno-6 is provided without any express or implied warranty.
 *     In no event shall UltraMaster Group be held liable for any damages
 *     arising from the use of UltraMaster Juno-6.
 */
/**
 * Copyright (c) UltraMaster Group, LLC. All Rights Reserved.
 * $Revision: 1.1 $$Date: 2004/06/21 15:03:36 $
 */
#ifndef CONTROL_H
#define CONTROL_H

#include "MoogObject.h"
//#include <libmoogutil/String.h>
#define SLIDER 0
#define FULLSLIDER 1
#define WAY2 2
#define WAY3 3
#define WAY4 4


class Control: public MoogObject
{
public:
	Control(Scheduler *sched);
	const char *getClassName()
	{
		return "Control";
	}
};

#endif /* JUNO_CONTROL_H */



