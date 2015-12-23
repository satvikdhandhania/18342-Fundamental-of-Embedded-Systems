@ user_mode_setup.S: User mode startup routine
@
@ Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
@         Vallari Mehta <vallarim@andrew.cmu.edu>  
@ Date:   Tue, 23 Oct 2015 11:20:33

#define USER_APPLICATION 0xa0000000
#define USER_STACK_BEGINNING 0xa3000000
    .file	"user_mode_setup.S"
	.text

	.global	user_mode_setup

user_mode_setup:

    stmfd sp!,{r8,lr}	@storing registers to stack
	@Enabling IRQ and FIQ for SVC mode
	mrs r3, cpsr   
	bic r3,r3,#0xc0	
	msr cpsr,r3    

	@Switching to USER MODE
	mrs r3, cpsr   
	bic r3,r3,#0xf @Clearing last 4 bits set mode to user	
	msr cpsr,r3    
	
    ldr sp,=USER_STACK_BEGINNING

    mov r4,r0	@moving number of arguments to r4
	cmp r4,#0	@comparing for no arguments
	beq  done  	@if no arguments then exit
loop:	
	add r2, r1, r4, LSL #2 
	ldr r5, [r2]
	str r5, [sp, #-4]!	@storing argv in top-4
	subs r4, r4, #1
	bge loop	
done:	
	str r0, [sp,#-4]!	@storing argc in stack top
    
    ldr pc,=USER_APPLICATION
