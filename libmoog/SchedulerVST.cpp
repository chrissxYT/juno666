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
#define _WIN32_WINNT 0x0400

#include <stdio.h>
#include <libmoogutil/debug.h>
#include <windows.h>
#include "Scheduler.h"
#include "GoObject.h"

Scheduler::Scheduler()
{
    nextGoHandle = 0;
    tickThread;
    controlRate = 0;
    sampleRate = 0;
    sampleControlRatio = DEFAULT_SAMPLE_CONTROL_RATIO;
    nyquistFreq = 44100.0 / 2.0;
    controlRateList.prev = &controlRateList;
    controlRateList.next = &controlRateList;
    sampleRateList.prev = &sampleRateList;
    sampleRateList.next = &sampleRateList;
}

Scheduler::~Scheduler()
{
}

void Scheduler::setSampleRate(int actual)
{
    //if (actual<2)

    sampleRate = actual;
    controlRate = sampleRate / sampleControlRatio;
    nyquistFreq = sampleRate / 2;
}

void Scheduler::safeListOp(list_head *node, list_head *list, bool add)
{
    if (add)
    {
        if (node->next != NULL)
        {
            debug(DEBUG_APPERROR, "Warn: obj already on scheduling list");
            return;
        }

        list_add(node, list);
    }
    else
    {
        if (node->next == NULL)
        {
            debug(DEBUG_APPERROR, "Warn: obj not on scheduling list");
            return;
        }

        /*
         * This is a bit of hack for the case where an object removes
         * itself from the list during the tick() walk of the linked
         * list.  If we clear the list_head that is the current iterator
         * in tick() then segfault we go.  So we shift the iter previous
         * here, then it jumps passed where we were in the next advance.
         */
        if (node == currentListIter)
        {
            currentListIter = currentListIter->prev;
        }

        list_del(node);
        CLEAR_LIST_NODE(node);
    }
}

void Scheduler::scheduleControlRate(GoObject *obj, bool schedule)
{
    if (schedule != obj->isControlScheduled())
        safeListOp(&obj->controlListNode, &controlRateList, schedule);
}

void Scheduler::scheduleSampleRate(GoObject *obj, bool schedule)
{
    if (schedule != obj->isSampleScheduled())
        safeListOp(&obj->sampleListNode, &sampleRateList, schedule);
}

void Scheduler::run()
{
    static int controlCount = 0;

    GoObject *obj;

    if (controlCount == 0)
    {
        currentListIter = controlRateList.next;

        while (currentListIter != &controlRateList)
        {
            obj = list_entry(currentListIter, GoObject, GoObject::controlListNode);
            obj->controlGo();
            currentListIter = currentListIter->next;
        }

        controlCount = sampleControlRatio;
    }

    controlCount--;

    currentListIter = sampleRateList.next;

    while (currentListIter != &sampleRateList)
    {
        if (currentListIter != NULL)
        {
            obj = list_entry(currentListIter, GoObject, sampleListNode);
            obj->sampleGo();
            currentListIter = currentListIter->next;
        }
        else
            break;
    }
}
