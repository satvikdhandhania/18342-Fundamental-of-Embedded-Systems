/*    @file run_queue.c
 *    Defines the logic to get the run_queue and highest priority in the 
 *    run_queue
 *    Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu> 
 *            Vallari Mehta <vallarim@andrew.cmu.edu>
 *    Date: 4th December 2015 
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <sched.h>
#include "sched_i.h"

#define NULL 0

static tcb_t* run_list[OS_MAX_TASKS]  __attribute__((unused));

/* A high bit in this bitmap means that the task whose priority is
 * equal to the bit number of the high bit is runnable.
 */
static uint8_t run_bits[OS_MAX_TASKS/8] __attribute__((unused));

/* This is a trie structure.  Tasks are grouped in groups of 8.  If any task
 * in a particular group is runnable, the corresponding group flag is set.
 * Since we can only have 64 possible tasks, a single byte can represent the
 * run bits of all 8 groups.
 */
static uint8_t group_run_bits __attribute__((unused));

/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[]  __attribute__((unused)) =
{

    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * Clears the run-queues and sets them all to empty.
 */
void runqueue_init(void)
{
    size_t i;
    group_run_bits=0;
//    uint8_t position_in_group, group;
    //Clears the run_queue
    for(i=0;i<OS_MAX_TASKS;i++)
    {
        run_list[i]=NULL;
    }
    //Clears the run_bits
    for(i=0;i<OS_MAX_TASKS/8;i++)
    {
        run_bits[i]=0;
    }
    /*
    // Create the IDLE TASK
    run_list[IDLE_PRIO] = &system_tcb[IDLE_PRIO];
    //Set bits for the IDLE TASK
    position_in_group = (IDLE_PRIO & 0x07);
    group = (IDLE_PRIO >> 3);

    run_bits[group] = run_bits[group] |(1<<position_in_group);
    group_run_bits = group_run_bits | (1<<group);
    */
}

/**
 * Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb  __attribute__((unused)), uint8_t prio  __attribute__((unused)))
{
    uint8_t position_in_group, group;
    //Adds the tcb to the run_list
    run_list[prio] = tcb;
    //Sets the corresposing bits in the run_bits and group bits
    position_in_group = (prio & 0x07);
    group = (prio >> 3);

    run_bits[group] = run_bits[group] |(1<<position_in_group);
    group_run_bits = group_run_bits | (1<<group);
}


/**
 * Empty the run queue of the given priority.
 *
 * The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t* runqueue_remove(uint8_t prio  __attribute__((unused)))
{
    tcb_t* removed_task;
    uint8_t position, group;
    // If IDLE task is tried to remove do not remove
    if(prio == IDLE_PRIO)
    {
        removed_task = run_list[IDLE_PRIO];
        return removed_task;
    }
    //Remove the task from run_list
    removed_task = run_list[prio];
    run_list[prio] = NULL;
    //Set bits according to task that was removed
    position = (prio & 0x07);
    group = (prio >> 3);


    run_bits[group] = (run_bits[group]&(~(1<<position)));
    // If bits are all 0 set it
    if(run_bits[group] == 0)
    {
        group_run_bits = (group_run_bits & (~(1 << group)));
    }

    return removed_task;
}

/**
 * This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void)
{
    uint8_t y = prio_unmap_table[group_run_bits];
    uint8_t x = prio_unmap_table[run_bits[y]];
    return  ((y << 3) + x);
}
