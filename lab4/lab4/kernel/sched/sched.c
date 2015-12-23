/** @file sched.c
 *     Defines the IDLE task, initializes the scheduler with IDLE task and the other provided tasks.
 *     Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu> 
 *     Vallari Mehta <vallarim@andrew.cmu.edu>
 *     Date: 4th December 2015
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>


#define NULL 0

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

/**
 * This is the idle task that the system runs when no other task is runnable
 */

static void __attribute__((unused)) idle(void)
{
    enable_interrupts();
    while(1);
}



void sched_init(task_t* main_task  __attribute__((unused)))
{
    sched_context_t *context;
    //Sets the priority and sleep queue for IDLE task   
    system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;        
    system_tcb[IDLE_PRIO].cur_prio = IDLE_PRIO;
    system_tcb[IDLE_PRIO].holds_lock = 0;
    system_tcb[IDLE_PRIO].sleep_queue = NULL;
    //Set the context    
    context = &(system_tcb[IDLE_PRIO].context);
    context->r4 = (unsigned)idle;
    context->r5 = 0;
    context->r6 = 0;
    context->sp = system_tcb[IDLE_PRIO].kstack_high;
    context->lr = launch_task;
	runqueue_add(&system_tcb[IDLE_PRIO],IDLE_PRIO);

}
void set_context(task_t *task, int prio)
{
    sched_context_t *context;
    //Sets the priority and sleep queue for IDLE task   
    system_tcb[prio].native_prio = prio;        
    system_tcb[prio].cur_prio = prio;
    system_tcb[prio].holds_lock = 0;
    system_tcb[prio].sleep_queue = NULL;
    //Set the context    
    context = &(system_tcb[prio].context);
    context->r4 = (unsigned)task->lambda;
    context->r5 = (unsigned)task->data;
    context->r6 = (unsigned)task->stack_pos;
    context->sp = system_tcb[prio].kstack_high;
    context->lr = launch_task;

}


/**
 * Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * param tasks is A list of scheduled task descriptors.
 * param size  is  The number of tasks is the list.
 */
void allocate_tasks(task_t** t  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
    size_t i;
    
    
    
    
    //Initialize Idle task sets the group_run_bits &  run_bits
    //sched_init(NULL);
    //Initializes the run_queue and adds Idle task
    //runqueue_init();
    //Used for easy referencing
    
    
    
    
    
    
    task_t *tasks = t[0];	
    for(i=1;i<=num_tasks;i++)
    {	
        //Setup TCB for task
        set_context((tasks+(i-1)),i);
        //Add tcb to runqueue
        runqueue_add(&system_tcb[i],i);
    }
    //Add the Idle task to the current running task list to have the lowest  priority
    dispatch_init(&system_tcb[IDLE_PRIO]);
}

