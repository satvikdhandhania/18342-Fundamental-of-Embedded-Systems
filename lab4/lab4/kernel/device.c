/*
 * Defines all the functions for the devices like init, wait and update.
 *Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu> 
 *Vallari Mehta <vallarim@andrew.cmu.edu>
 *Date: 4th December 2015
 */

#include <types.h>
#include <assert.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

#define NULL 0
extern volatile unsigned long timer_count;


/**
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
    tcb_t* sleep_queue;
    unsigned long   next_match;
};
typedef struct dev dev_t;

/* Devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * Initializing the sleep queues and match values for all devices.
 */
void dev_init(void)
{
    int i;
    for(i=0;i<NUM_DEVICES;i++)
    {
        devices[i].next_match =timer_count + dev_freq[i];
        devices[i].sleep_queue = NULL;
    }

}


/**
 * Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * param dev is Device number.
 */
void dev_wait(unsigned int dev __attribute__((unused)))
{
    disable_interrupts();

    //tcb of current running task
    tcb_t *cur_tcb = get_cur_tcb();
    tcb_t *temp = devices[dev].sleep_queue;

    /*
    if(cur_tcb->holds_lock>0)
        return 
    */
    
    
    //Add first element to sleep queue
    if(temp==NULL)
    {
        devices[dev].sleep_queue = cur_tcb;
        devices[dev].sleep_queue->sleep_queue = NULL;
    }
    else
    {//Add to the end of the sleep queue
        while(temp->sleep_queue != 0)
        {
            temp = temp->sleep_queue;
        }
        temp->sleep_queue = cur_tcb;
        temp = temp->sleep_queue;
        temp->sleep_queue = NULL;
    }
    dispatch_sleep();
    enable_interrupts();
}


/**
 * Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis __attribute__((unused)))
{

    //disable_interrupts();

    int i, dispatch_update_flag = 0;
    tcb_t *next_node = NULL;
    tcb_t *sleep_node = NULL;
    for (i = 0; i < NUM_DEVICES; i++) 
    {

        //If devices match current system time
        if (devices[i].next_match <= millis) 
        {
            // update next match
            devices[i].next_match = millis + dev_freq[i];

            sleep_node = devices[i].sleep_queue;
            devices[i].sleep_queue = NULL;
            //Put all the tcbs of that device from sleep queue to runqueue
            while (sleep_node != NULL) 
            {
                dispatch_update_flag = 1;
                //Add node to runqueue
                runqueue_add(sleep_node, sleep_node->cur_prio);

                //Get next task in device sleep_queue
                next_node = sleep_node->sleep_queue;

                //got to next sleep task
                sleep_node->sleep_queue = NULL;

                //set current task next to null
                sleep_node = next_node;

            }

        }
    }
    //If nodes are added to runqueue context switch immediately to get highest priority task running
    if(dispatch_update_flag)
        dispatch_save();
}
