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
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <lock.h>
//Imported from Lab3
#include <exports.h>
#include <arm/psr.h>
#include <arm/reg.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>



#define SWI_VECTOR 0x08
#define PC_OFFSET 0x08
#define MASK_ONE 0x1
#define OFFSET_MASK 0xFFF
#define POSITIVE_OFFSET 0xe51ff000
#define NEGATIVE_OFFSET 0xe59ff000
#define NEW_INSTRUCTION1 0xe51ff004
#define MASK_CHECK_LDR 0xFF7FF000
#define IRQ_VECTOR 0x18
#define TEN_MILLIS_MATCH_VALUE 32500
#define SET_SWI 1
#define SET_IRQ 2


//Storing U-boot registers
uint32_t stack_pointer, link_register;

//SWI Restore variables
unsigned int swi_instruction1;
unsigned int swi_instruction2;
unsigned int *swi_address;

//IRQ Restore variables
unsigned int irq_instruction1;
unsigned int irq_instruction2;
unsigned int *irq_address;

//Timer Restore Variables
//volatile unsigned long timer_count = 0;
unsigned long CPSR;
unsigned long OSMR0; // OS_TIMER_MATCH_REGISTER_0
unsigned long OIER;  // OS_TIMER_INTERRUPT_ENABLE_REGISTER
unsigned long ICLR;  // Interrupt Controller Level Register
unsigned long ICMR;  // Interrupt Controller Mask Register

extern void irq_stack_setup();
extern void user_mode_setup();
extern void swihandler();
extern void irq_wrapper();
extern void set_timer();
uint32_t global_data;
extern int hijacking(unsigned int base_vector, int mode);


int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table, uint32_t spointer ,uint32_t lregister)
{

    app_startup();
    global_data = table;
    /* add your code up to assert statement */

    stack_pointer = spointer; // Storing stack pointer
    link_register = lregister;  //storing link register

    //Replacing SWI handler with our custom handler
    if(hijacking(SWI_VECTOR, SET_SWI )==0xbadc0de)
        return 0xbadc0de;
    //Replacing IRQ handler with our custom handler
    if(hijacking(IRQ_VECTOR, SET_IRQ )==0xbadc0de)
        return 0xbadc0de;

    //Setting timer variable as per our applications need
    set_timer();
    mutex_init();
    user_mode_setup(argc,argv); //Initializes stack for the user 
    return -255;

    assert(0);        /* should never get here */
}

