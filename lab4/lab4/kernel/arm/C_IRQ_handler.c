/** @file main.c
 *
 * kernel.c: Kernel main (entry) function
 *
 * Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu> 
 *	   Vallari Mehta <vallarim@andrew.cmu.edu> 
 *	   
 * Date:   25th Nov 2015
 */


//Given in src code
#include <kernel.h>
#include <arm/psr.h>
#include <arm/reg.h>
#include <arm/timer.h>
#include <device.h>
#include <arm/interrupt.h>
#define TEN_MILLIS 32500
#define TIME_RESOLUTION 10

volatile unsigned long timer_count = 0;

void C_IRQ_handler()
{
    uint32_t interruptMR0 = reg_read(INT_ICPR_ADDR) & (1 << (INT_OSTMR_0));
    if(interruptMR0)
    {
        //Increasing global variable value
        timer_count += TIME_RESOLUTION;
        //Clearing OSSR register
        reg_set(OSTMR_OSSR_ADDR,OSTMR_OSSR_M0);
        //*OSMR0 = OSMR0 + 32500 ;    
        reg_write(OSTMR_OSMR_ADDR(0),reg_read(OSTMR_OSMR_ADDR(0))+TEN_MILLIS);

        dev_update(timer_count);
    }
}
