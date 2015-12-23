/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 * @ Author Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 *          Vallari Mehta <vallarim@andrew.cmu.edu>
 *
 *         @date : 2 Dec 2015
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>

extern volatile unsigned long timer_count;


unsigned long time_syscall(void)
{

    return timer_count;

}



/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep.
 *
 * 
 */
void sleep_syscall(unsigned long millis  __attribute__((unused)))
{
    unsigned long end_time = timer_count + millis;
    while(timer_count < end_time)
        ;	
}
