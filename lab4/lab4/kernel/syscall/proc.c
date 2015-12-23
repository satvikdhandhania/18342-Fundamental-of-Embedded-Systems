/** @file proc.c
 *  Defines task_create and event_wait syscall.
 *
 *  Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu> 
 *          Vallari Mehta <vallarim@andrew.cmu.edu>
 *  Date: 4th December 2015
 *
 * Implementation of `process' syscalls
 *
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <../sched/sched_i.h>



#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>


#define NULL 0

// Implements task_create, sorts and sends tasks to assign schedule to determine schedulabilty initializes runqueue
// and devices.

int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
    disable_interrupts();
    size_t i,j,min,index;
    task_t temp;
    // If tasks are more than what the kernel supports return error
    // The kernal supports only 62 tasks as 0 is reserved for highest priority task
    // Thus we check for OS_AVAIL_TASKS for num_tasks equal to OS_AVAIL_TASKS
    if(num_tasks >= OS_AVAIL_TASKS || num_tasks <= 0)
        return -EINVAL;
    //If the tasks are stored outside the user address space
    if(!valid_addr(tasks,sizeof(task_t)*num_tasks,USR_START_ADDR, USR_END_ADDR))
        return -EFAULT;
    //If the stack pointers of the tasks are outside the range for the user
    for(i=0; i<num_tasks; i++)
    {
        if((((unsigned)(tasks[i].stack_pos)) < USR_START_ADDR) || (((unsigned)(tasks[i].stack_pos)) > USR_END_ADDR))
            return -EFAULT;
    }
    //Sort tasks to run based on priority
    
    for(i=0;i<num_tasks;i++)
    {
        min = tasks[i].T;
        index = i;
        for(j=i+1; j<num_tasks;j++)
        {
            if(tasks[j].T < min)
            { 
                min = tasks[j].T;
                index = j;
            }
        }
        if(index!=i)
        {
            temp = tasks[i];
            tasks[i]=tasks[index];
            tasks[index]= temp;
        }
    }
   

    runqueue_init();
    //Initializes all the devices
    dev_init();

    if (assign_schedule(&tasks, num_tasks) == 0)
    {
        printf("\nScheduling failed!!\n");
        return -ESCHED;
    }

    //Initialize tcb for each task
    allocate_tasks(&tasks,num_tasks);

    sched_init(NULL);

    enable_interrupts();
    //To set idle task as the first task and get the lowest priority
    dispatch_nosave();
    return 1;
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
    tcb_t *cur = get_cur_tcb();

    if(dev > NUM_DEVICES)
    {
        return -EINVAL;
    }

    if(cur->holds_lock > 0)
    {
        return -EHOLDSLOCK;
    }

    dev_wait(dev);

    return 0; 
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
    printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

    disable_interrupts();
    while(1);
}
