/*
Author : Satvik Dhandhania <sdhandha@andrew.cmu.edu>
Vallari Mehta <vallarim@andrew.cmu.edu>
Date : 24th Oct 2015
*/
#include "C_handler.h"
#include<bits/swi.h>
#include<bits/fileno.h>
#include<exports.h>
#include<bits/errno.h>
#include<arm/reg.h>
#include<arm/timer.h>
#include<syscall.h>
#include <lock.h>
#define NULL_CHAR 0
#define END_OF_TRANSMISSION 4
#define BACKSPACE 8
#define DELETE 127
#define NEW_LINE 10
#define CARRAIGE_RETURN 13
#define START_SDRAM 0xa0000000
#define END_SDRAM 0xa3ffffff	
#define START_STRATA 0x00000000
#define END_STRATA 0x00ffffff

void C_SWI_handler(int swi_num, unsigned *regs)
{

    switch(swi_num)
    {	//Memory mapped registers
        case READ_SWI: regs[0]=read_syscall(regs[0],(char *)regs[1],regs[2]); 
                       break;

        case WRITE_SWI:regs[0]=write_syscall(regs[0],(char *)regs[1],regs[2]);
                       break;

        case TIME_SWI: regs[0] = time_syscall();
                       break;

        case SLEEP_SWI: sleep_syscall(regs[0]);
                        break;

        case CREATE_SWI: regs[0]=task_create((task_t *)regs[0],(size_t) regs[1]);
                         break;

        case MUTEX_CREATE: regs[0]=mutex_create();
                           break;

        case MUTEX_LOCK: regs[0]=mutex_lock(regs[0]);
                         break;

        case MUTEX_UNLOCK:regs[0]=mutex_unlock(regs[0]);
                          break;

        case EVENT_WAIT: regs[0]=event_wait(regs[0]);
                         break;

        default:puts("Invalid Instruction!! Exiting!");
                break;
    }
}

