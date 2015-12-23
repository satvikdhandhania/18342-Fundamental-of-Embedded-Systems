/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 *         Vallari Mehta <vallarim@andrew.cmu.edu> 		
 * Date:   24th Oct 2015
 */
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
volatile unsigned long timer_count = 0;
unsigned long CPSR;
unsigned long OSMR0; // OS_TIMER_MATCH_REGISTER_0
unsigned long OIER;  // OS_TIMER_INTERRUPT_ENABLE_REGISTER
unsigned long ICLR;  // Interrupt Controller Level Register
unsigned long ICMR;  // Interrupt Controller Mask Register

void irq_stack_setup();
void user_mode_setup();
void swihandler();
void irqhandler();
void set_timer();
uint32_t global_data;
int hijacking(unsigned int base_vector, int mode);

int kmain(int argc, char** argv, uint32_t table, uint32_t spointer ,uint32_t lregister)
{
    app_startup(); /* bss is valid after this point */
	global_data = table;      //Storing Jump table
    stack_pointer = spointer; // Storing stack pointer
    link_register = lregister;  //storing link register
    
    //Replacing SWI handler with our custom handler
    if(hijacking(SWI_VECTOR, SET_SWI )==0xbadc0de)
        return 0xbadc0de;
    //Replacing IRQ handler with our custom handler
    if(hijacking(IRQ_VECTOR, SET_IRQ )==0xbadc0de)
        return 0xbadc0de;
    
    irq_stack_setup();
    //Setting timer variable as per our applications need
    set_timer(); 

    user_mode_setup(argc,argv);	//Initializes stack for the user 
	return -255;
}
void set_timer()
{
	unsigned mask_timer;
    //Restarting OSCR
    reg_write(OSTMR_OSCR_ADDR,0);
    
    //Saving OSMR0 
    OSMR0 = reg_read(OSTMR_OSMR_ADDR(0));
    //Setting timer precision and values
    reg_write(OSTMR_OSMR_ADDR(0),TEN_MILLIS_MATCH_VALUE);
    
    //Saving OIER
    OIER = reg_read(OSTMR_OIER_ADDR);
    //Enabling OIER for MATCH REGISTER 0
    reg_set(OSTMR_OIER_ADDR,OSTMR_OIER_E0);
    //Clearing OSSR register for match register 0
    reg_set(OSTMR_OSSR_ADDR,OSTMR_OSSR_M0); 
    //Saving ICLR and ICMR
    ICMR = reg_read(INT_ICLR_ADDR);
    ICLR = reg_read(INT_ICMR_ADDR);

    mask_timer = MASK_ONE << INT_OSTMR_0;
    // Clearing ICLR register and setting ICMR register to generate IRQ's
    reg_clear(INT_ICLR_ADDR,mask_timer);
    reg_set(INT_ICMR_ADDR,mask_timer);

}
void restore_timer()
{
    
    //Restoring OSMR0 match register
    reg_write(OSTMR_OSMR_ADDR(0),OSMR0);
    
    //Restoring OIER match register
    reg_write(OSTMR_OIER_ADDR,OIER);

    //Restoring OSMR0 match register
    reg_write(INT_ICLR_ADDR,ICLR);

    //Restoring OSMR0 match register
    reg_write(INT_ICMR_ADDR,ICMR);


}


int hijacking(unsigned int base_vector, int mode)
{


	unsigned int  *vector = (unsigned *)base_vector; 
	unsigned int temp;
	unsigned int offset = (*vector & OFFSET_MASK);/* calculate branch 
                                              offset from instruction*/
	//error handling for non-LDR instructions
	unsigned int error_check= (*vector&MASK_CHECK_LDR);
	int flag=0;
    //Checking whether LDR is valid or not
	if(((error_check^NEGATIVE_OFFSET)==0)||((error_check^POSITIVE_OFFSET)==0))
              flag=1;
	
	if(flag)
    {
        temp = (unsigned int) vector; 
        if((error_check^NEGATIVE_OFFSET)==0)
        {
            if(offset> (temp+base_vector))
                temp = temp+ PC_OFFSET - offset;
            else 
                return 0;	// Terminates if value goes under 0 address
        }
        else
        {	
            temp = temp+ PC_OFFSET + offset;
        }

        unsigned int *softvec = (unsigned int*)temp;
        unsigned int new_instruction1 = NEW_INSTRUCTION1; //New 1st line for handler
        unsigned int *inst = (unsigned *) *softvec;
        //Replacing SWIHANDLER
        if(mode == 1)
        {
            swi_address = inst;
            swi_instruction1 = *inst;	// Storing 1st instruction in global variable
            swi_instruction2 = *(inst+1); //storing 2nd instruction in global variable
            *inst = new_instruction1;  //Overwritin out new first instruction
            *(inst+1) = (unsigned) &swihandler;  // saving location of new handler
        }
        //Replacing IRQHANDLER
        if(mode == 2)
        {
            irq_address = inst;
            irq_instruction1 = *inst;	// Storing 1st instruction in global variable
            irq_instruction2 = *(inst+1); //storing 2nd instruction in global variable
            *inst = new_instruction1;  //Overwritin out new first instruction
            *(inst+1) = (unsigned) &irqhandler;  // saving location of new handler
        }

    }
    else
    {
        return 0xbadc0de;	
    }
    return 1;
}


void C_IRQ_handler()
{
    //Increasing global variable value
    timer_count++;
    //Clearing OSSR register
    reg_set(OSTMR_OSSR_ADDR,OSTMR_OSSR_M0);
    //*OSCR0 = 0;    
    reg_write(OSTMR_OSCR_ADDR,0);
}
