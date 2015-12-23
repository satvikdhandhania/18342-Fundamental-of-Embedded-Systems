/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 * Date:   24th Oct 2015
 */
#include "C_SWI_handler.h"
unsigned int instruction1;
unsigned int instruction2;
unsigned int *address;
unsigned int stack_pointer, link_register, r_eight;

void user_mode_setup();
void swihandler();
#define SWI_VECTOR 0x08
#define OFFSET_MASK 0xFFF
#define OFFSET_MASK 0xFFF
#define POSITIVE_OFFSET 0xe51ff000
#define NEGATIVE_OFFSET 0xe59ff000
#define NEW_INSTRUCTION1 0xe51ff004
#define MASK_CHECK_LDR 0xFF7FF000

int main(int argc, char *argv[]) {
	/* Needs more code. */
	unsigned int  *vector = (unsigned *)SWI_VECTOR;  //vector<-0x08
	unsigned int temp;
	unsigned int offset = (*vector & OFFSET_MASK);   //Calculate offset
	// error handling for non-LDR instructions
	unsigned int error_check= (*vector&MASK_CHECK_LDR);
	int flag=0;
	if(((error_check^NEGATIVE_OFFSET)==0)||((error_check^POSITIVE_OFFSET)==0))
              flag=1;
	
	if(flag)
	{
	 temp = (unsigned int) vector; 
	 if((error_check^NEGATIVE_OFFSET)==0)
	 {
	  if(offset> (temp+SWI_VECTOR))
		temp = temp+ SWI_VECTOR - offset;
          else 
		return 0;	// Terminates if value goes under 0 address
	 }
	 else
	 {	
	   temp = temp+ SWI_VECTOR + offset;
	 }
	unsigned int *softvec = (unsigned int*)temp;
	unsigned int new_instruction1 = NEW_INSTRUCTION1; //New 1st line for handler
	unsigned int *inst = (unsigned *) *softvec;
	address = inst;
	instruction1 = *inst;	// Storing 1st instruction in global variable
	instruction2 = *(inst+1); //storing 2nd instruction in global variable
	*inst = new_instruction1;  //Overwritin out new first instruction
	*(inst+1) = (unsigned) &swihandler;  // saving location of new handler
	user_mode_setup();	//Initializes stack for the user 
	}
	else
	{
	  return 0xbadc0de;	
	}
	return -255;
}
