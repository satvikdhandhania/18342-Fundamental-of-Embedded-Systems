/*   @file ctx_switch.c
 *   Defines all the dispatch functions. 
 *
 *   Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu> 
 *           Vallari Mehta <vallarim@andrew.cmu.edu>
 *   Date: 4th December 2015
 */

#include <types.h>
#include <assert.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif


static __attribute__((unused)) tcb_t* cur_tcb; 

/**
 * Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{   //Assigning idle task just for the sake of comparison for the first dispatch_save.DOES NOT LAUNCH IT.
    cur_tcb = idle;
}


/**
 * Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{   
    //save the current task state
    tcb_t* previous_tcb = cur_tcb;
    //Check whether this condition is correct or needed or not
    if(cur_tcb->cur_prio>highest_prio())
    {    

        runqueue_add(previous_tcb, previous_tcb->cur_prio);
        //make current task as highest priority task
        cur_tcb = runqueue_remove(highest_prio());
        //context switch to highest priority task
        ctx_switch_full(&cur_tcb->context, &previous_tcb->context);

    }
}   
/**
 * Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 */
void dispatch_nosave(void)
{   
    cur_tcb = runqueue_remove(highest_prio());
    ctx_switch_half(&cur_tcb->context);
}


/**
 * Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
    //save the current task state
    tcb_t* previous_tcb = cur_tcb;

    cur_tcb = runqueue_remove(highest_prio());
    //context switch to highest priority task
    ctx_switch_full(&cur_tcb->context , &previous_tcb->context);
}

/**
 * Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
    return cur_tcb->cur_prio; 
}

/**
 * Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
    return cur_tcb; 
}
