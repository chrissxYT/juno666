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
#include <stdio.h>
#include <stdlib.h>
#include <libmoogutil/debug.h>
#include <libmoogutil/rcsid.h>
#include "GoObject.h"
#include "Scheduler.h"

RCSID("$Id: GoObject.cpp,v 1.4 2004/04/20 15:01:35 brainslayer Exp $");



#include "MoogObject.h"

GoObject::GoObject(Scheduler *sched)
{
	schedule = sched;
	
	controlListNode.next = NULL;
	controlListNode.prev = NULL;
	
	sampleListNode.next = NULL;
	sampleListNode.prev = NULL;

	goHandle = schedule->nextGoHandle++;
}

bool GoObject::isControlScheduled()
{
	return controlListNode.next != NULL;
}

bool GoObject::isSampleScheduled()
{
	return sampleListNode.next != NULL;
}

void GoObject::controlGo()
{
	//FIXME: cast GoObject as MoogObject
	debug(DEBUG_APPERROR, "GoObject::controlGo should never get called! [%s]",
		((MoogObject *)this)->getClassName());
}

void GoObject::sampleGo()
{
	//FIXME: cast GoObject as MoogObject
	debug(DEBUG_APPERROR, "GoObject::sampleGo should never get called! [%s]",
		((MoogObject *)this)->getClassName());
}

