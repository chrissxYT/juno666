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
 * $Revision: 1.6 $$Date: 2004/04/24 22:24:10 $
 */
#ifndef _GOOBJECT_H
#define _GOOBJECT_H

#include <libmoogutil/list.h>
#include <libmoogutil/SimpleArray.h>

/* $Id: GoObject.h,v 1.6 2004/04/24 22:24:10 strepto Exp $ */

class GoObject
{
    friend class Scheduler;

    int nextGoHandle;

    list_head controlListNode;
    list_head sampleListNode;

protected:
    int goHandle;

    bool isControlScheduled();
    bool isSampleScheduled();

public:
    Scheduler *schedule;
    GoObject(Scheduler *schedule);
    virtual void controlGo();
    virtual void sampleGo();

    void on();
    void off();
};

#endif /* _GOOBJECT_H */

