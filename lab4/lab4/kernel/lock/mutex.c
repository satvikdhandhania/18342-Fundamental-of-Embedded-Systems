/*
   Defines mutex_init, mutex_create and mutex_lock/unlock to provide
   the user with the mutex system call.

Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu> 
Vallari Mehta <vallarim@andrew.cmu.edu>
Date: 4th November 2015
*/

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

#define NULL 0

mutex_t gtMutex[OS_NUM_MUTEX];
//Initializes all mutexes for the initial stage, called by kernel's main at the beginning
void mutex_init()
{   
    int i;
    //initialize all mutexes to be available and unlocked
    for(i=0; i< OS_NUM_MUTEX;i++)
    {
        gtMutex[i].bAvailable = TRUE;
        gtMutex[i].pHolding_Tcb = NULL;
        gtMutex[i].bLock = FALSE;
        gtMutex[i].pSleep_queue = NULL;
    }

}
/* Called by user programs to allocate a mutex from the available mutexes.
 * It first checks for the availability and then returns the index position 
 * so that it can be referenced in the future. Once allocated it will never be
 * available again.
 */
int mutex_create(void)
{
    int i;
    //Disabling interrupts so that no other tasks can interfere in between
    disable_interrupts();
    for(i=0;i<OS_NUM_MUTEX;i++)
    {   //Checking if the mutex is not already taken
        if(gtMutex[i].bAvailable == TRUE)
        {        
            //Change the availabilty as this mutex is taken
            gtMutex[i].bAvailable = FALSE;
            //enable interrupts as this is a safe point
            enable_interrupts();
            return i;
        }

    }
    //Re-enable interrupts
    enable_interrupts();
    //Control reaches here only when no mutex is available (equals OS_NUM_MUTEX)
    return -ENOMEM; 
}
/*
 *Check various conditions when user cannot lock a mutex and if legible allows the user 
 *to capture the mutex. Interrupts are handled in the process so that other tasks do not
 *interfere. As soon as a task locks a mutex it's priority becomes 0 that is the highest
 *priority.
 */

int mutex_lock(int mutex  __attribute__((unused)))
{
    //Disable interrupts so that no tasks interfere in between the locking process
    disable_interrupts();
    tcb_t *temp = NULL;
    tcb_t *cur_tcb = get_cur_tcb();
    //If mutex index is invalid enable interrupts and return EINVAL 
    if((mutex<0)||(mutex>=OS_NUM_MUTEX))
    {
        enable_interrupts();
        return -EINVAL;
    }
    //If mutex is still available it should not be locked
    if(gtMutex[mutex].bAvailable)
    {
        enable_interrupts();
        return -EINVAL;
    }

    //Check if this is already holding the lock it will be deadlock
    if(gtMutex[mutex].pHolding_Tcb == cur_tcb)
    {   
        enable_interrupts();
        return -EDEADLOCK;
    }
    //If mutex is blocked
    if(gtMutex[mutex].bLock == TRUE)
    {   
        //If no task is waiting for the mutex, add first task to sleep queue
        if(gtMutex[mutex].pSleep_queue == NULL)
        {
            //Adding cur_tcb to sleep_queue
            gtMutex[mutex].pSleep_queue = cur_tcb;
            cur_tcb->sleep_queue = NULL;
        }
        else
        {   //Otherwise we add it to the end of the list
            temp = gtMutex[mutex].pSleep_queue;
            //traverse to the last tcb
            while(temp->sleep_queue!=NULL)
            {
                temp = temp->sleep_queue;
            }
            //Add tcb to the last position 
            temp->sleep_queue = cur_tcb;
            cur_tcb->sleep_queue = NULL;
        }
        //Sleep current task
        dispatch_sleep();
    }
    //Dispatch sleep will return here only if task is added back to the runqueue
    //which will happen only when the mutex is released. If it is readily available
    //it is directly assigned
    gtMutex[mutex].bLock = TRUE;
    gtMutex[mutex].pHolding_Tcb = cur_tcb;
    cur_tcb->holds_lock = cur_tcb->holds_lock + 1;       
    cur_tcb->cur_prio = 0;
    enable_interrupts();
    return 0;
}
/*
 *Check's for various conditions where a task may be unable to unlock the mutex.
 * If possible it unlocks the mutex and if the current task is not holding any 
 * mutex it restores its current priority back to the native priority.
 *
 */
int mutex_unlock(int mutex  __attribute__((unused)))
{
    tcb_t *temp;
    //disable interrupts to avoid interruptions
    disable_interrupts();
    tcb_t *cur_tcb = get_cur_tcb();

    //If mutex index is invalid enable interrupts and return EINVAL 
    if((mutex<0)||(mutex>=OS_NUM_MUTEX))
    {
        enable_interrupts();
        return -EINVAL;
    }
    //If mutex is still available it should not be unlocked
    if(gtMutex[mutex].bAvailable)
    {
        enable_interrupts();
        return -EINVAL;
    }
    //Automatically checks whether locked or not too
    //Check if this not holding the lock
    if(gtMutex[mutex].pHolding_Tcb != cur_tcb)
    {
        enable_interrupts();
        return -EPERM;
    }
    //Unlocking the mutex
    gtMutex[mutex].bLock = FALSE;
    gtMutex[mutex].pHolding_Tcb = NULL;
    //Decreasing the lock count in the tcb
    cur_tcb->holds_lock = cur_tcb->holds_lock - 1;
    if(cur_tcb->holds_lock == 0)
        cur_tcb->cur_prio = cur_tcb->native_prio;

    //If the sleep queue is not empty add the first task to the run_queue
    if(gtMutex[mutex].pSleep_queue != NULL)
    {
        temp = gtMutex[mutex].pSleep_queue;
        gtMutex[mutex].pSleep_queue = temp->sleep_queue;
        temp->sleep_queue = NULL;

        runqueue_add(temp,temp->cur_prio);

        //Do we call dipatch save to get a higher priority released task to run
        dispatch_save();
    }
    //Zero is returned on successful execution
    enable_interrupts();
    return 0; 
}

