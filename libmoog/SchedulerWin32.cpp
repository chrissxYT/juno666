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
#include "DSPOutput.h"

static void dataWrittenCallback();
static int set_realtime_priority();

static volatile int needListSync = 0;
static list_head *currentListIter;
static CRITICAL_SECTION beginListOpMutex;

DWORD ThreadID;
typedef HANDLE pthread_mutex_t;

typedef struct
{
    int waiters_count_;
    // Number of waiting threads.

    CRITICAL_SECTION waiters_count_lock_;
    // Serialize access to <waiters_count_>.

    HANDLE sema_;
    // Semaphore used to queue up threads waiting for the condition to
    // become signaled.

    HANDLE waiters_done_;
    // An auto-reset event used by the broadcast/signal thread to wait
    // for all the waiting thread(s) to wake up and be released from the
    // semaphore.

    size_t was_broadcast_;
    // Keeps track of whether we were broadcasting or signaling.  This
    // allows us to optimize the code if we're just signaling.
} pthread_cond_t;

int
pthread_cond_init(pthread_cond_t *cv)
{
    cv->waiters_count_ = 0;
    cv->was_broadcast_ = 0;
    cv->sema_ = CreateSemaphore(NULL, // no security
        0, // initially 0
        0x7fffffff, // max count
        NULL); // unnamed
    InitializeCriticalSection(&cv->waiters_count_lock_);
    cv->waiters_done_ = CreateEvent(NULL, // no security
        FALSE, // auto-reset
        FALSE, // non-signaled initially
        NULL); // unnamed
    return 0;
}

int
pthread_cond_wait(pthread_cond_t *cv, HANDLE *external_mutex)
{
    // Avoid race conditions.
    EnterCriticalSection(&cv->waiters_count_lock_);
    cv->waiters_count_++;
    LeaveCriticalSection(&cv->waiters_count_lock_);

    // This call atomically releases the mutex and waits on the
    // semaphore until <pthread_cond_signal> or <pthread_cond_broadcast>
    // are called by another thread.
    SignalObjectAndWait(*external_mutex, cv->sema_, INFINITE, FALSE);

    // Reacquire lock to avoid race conditions.
    EnterCriticalSection(&cv->waiters_count_lock_);

    // We're no longer waiting...
    cv->waiters_count_--;

    // Check to see if we're the last waiter after <pthread_cond_broadcast>.
    int last_waiter = cv->was_broadcast_ && cv->waiters_count_ == 0;

    LeaveCriticalSection(&cv->waiters_count_lock_);

    // If we're the last waiter thread during this particular broadcast
    // then let all the other threads proceed.
    if (last_waiter)
        // This call atomically signals the <waiters_done_> event and waits until
        // it can acquire the <external_mutex>.  This is required to ensure fairness.
        SignalObjectAndWait(cv->waiters_done_, *external_mutex, INFINITE, FALSE);
    else
        // Always regain the external mutex since that's the guarantee we
        // give to our callers.
        WaitForSingleObject(*external_mutex, INFINITE);
    return 0;
}

int
pthread_cond_signal(pthread_cond_t *cv)
{
    EnterCriticalSection(&cv->waiters_count_lock_);
    int have_waiters = cv->waiters_count_ > 0;
    LeaveCriticalSection(&cv->waiters_count_lock_);

    // If there aren't any waiters, then this is a no-op.
    if (have_waiters)
        ReleaseSemaphore(cv->sema_, 1, 0);
    return 0;
}

static pthread_cond_t listOpCompleteCond;

Scheduler::Scheduler()
{
    dsp = NULL;

    suspended = 0;
    nextGoHandle = 0;
    tickThread;
    controlRate = 0;
    sampleRate = 0;
    sampleControlRatio = DEFAULT_SAMPLE_CONTROL_RATIO;
    nyquistFreq = 44100 / 2;
    controlRateList.prev = &controlRateList;
    controlRateList.next = &controlRateList;
    sampleRateList.prev = &sampleRateList;
    sampleRateList.next = &sampleRateList;

    pthread_cond_init(&listOpCompleteCond);
    InitializeCriticalSection(&beginListOpMutex);
}

Scheduler::~Scheduler()
{
    stop();

    DeleteCriticalSection(&beginListOpMutex);
}

void Scheduler::setSampleRate(int actual)
{
    sampleRate = actual;
    controlRate = sampleRate / sampleControlRatio;
    nyquistFreq = sampleRate / 2;
}

void Scheduler::setSampleControlRatio(int r)
{
    sampleControlRatio = r;

    /* force some recalculation done in setSampleRate */
    setSampleRate(sampleRate);
}

void Scheduler::safeListOp(list_head *node, list_head *list, bool add)
{
    needListSync++;
    EnterCriticalSection(&beginListOpMutex);
    /* since the synth thread runs with the beginListOpMutex locked,
     * getting here means that that thread is now waiting on the condition
     * variable, so we are safe to proceed, or this IS the synth thread
     * since the mutex is a recursive lock
     */

    if (add)
    {
        if (node->next != NULL)
        {
            debug(DEBUG_APPERROR, "Warn: obj already on scheduling list");
            goto out;
        }

        //debug( DEBUG_STATUS, "Adding %s to scheduling list",
        //      ((MoogObject*)list_entry(node, GoObject, sampleListNode ))->getClassName() );

        list_add(node, list);
    }
    else
    {
        if (node->next == NULL)
        {
            debug(DEBUG_APPERROR, "Warn: obj not on scheduling list");
            goto out;
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

out:
    needListSync--;
    pthread_cond_signal(&listOpCompleteCond);

    LeaveCriticalSection(&beginListOpMutex);


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

int running = 0;
static Scheduler *sched;
void Scheduler::start(DSPOutput *_dsp)
{
    dsp = _dsp;

    if (dsp && dsp->isOpen())
        dsp->setDataWrittenCallback(dataWrittenCallback);
    else
        dsp = NULL;
    running = 1;


	sched = this;
    tickThread = CreateThread(NULL, 0, runSynth, NULL, 0, &ThreadID);

//    pthread_create(&tickThread, NULL, (DWORD *()(void*)&runSynth, NULL);
}

void Scheduler::stop()
{
    running = 0;
//    pthread_cancel(tickThread);
//    pthread_join(tickThread, NULL);
}

void Scheduler::suspend()
{
//    suspended = 1;
}

void Scheduler::resume()
{
//puts("try to resume");
//EnterCriticalSection(&beginListOpMutex);
//puts("ready");
//    suspended = 0;
//    pthread_cond_signal(&resumeCondition);
//puts("try to leave resume");
//LeaveCriticalSection(&beginListOpMutex);
//puts("ready");
}


DWORD CALLBACK runSynth(void *)
{
    sched->run();
    /* not reached */
    return 0;
}

void Scheduler::run()
{
    int controlCount = 0;
    GoObject *obj;

//EnterCriticalSection(&beginListOpMutex);
    while (running)
    {


        if (controlCount == 0)
        {
            currentListIter = controlRateList.next;

            while (currentListIter != &controlRateList)
            {
                if (!currentListIter)
                {
                    break;
                }

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
            if (!currentListIter)
            {
                break;
            }

            obj = list_entry(currentListIter, GoObject, sampleListNode);
            obj->sampleGo();
            currentListIter = currentListIter->next;
        }

        /* this is so we can run even without a DSPOutput object */
        if (!dsp && needListSync > 0)
        {
            pthread_cond_wait(&listOpCompleteCond, (HANDLE *)&beginListOpMutex);
        }
    }
}

/* NOTE: the dataWrittenCallback will be called from the runSynth thread
 *       from dsp->sampleGo()
 *       from Scheduler::run()
 *
 * This routine is essentially a convenient point for the tick thread
 * to block so that the linked lists con be manipulated by another thread.
 */

void dataWrittenCallback()
{
    if (needListSync > 0)
    {
        pthread_cond_wait(&listOpCompleteCond, (HANDLE *)&beginListOpMutex);
    }
}


/* this routine taken from Benno Sonner, sbenno@gardena.net */

int set_realtime_priority()
{
/*    struct sched_param schp;

    memset(&schp, 0, sizeof(schp));
    schp.sched_priority = sched_get_priority_max(SCHED_FIFO);

    if (sched_setparam(0,&schp) != 0)
    return -1;
*/
    return 0;
}
