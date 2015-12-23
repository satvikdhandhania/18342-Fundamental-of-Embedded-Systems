@ user_mode_setup.S: User mode startup routine
@
@ Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
@ Date:   Tue, 23 Oct 2015 11:20:33

	.file	"user_mode_setup.S"
	.text

	.global	user_mode_setup

user_mode_setup:

        stmfd sp!,{r8,lr}	@storing registers to stack
	
	mrs r3, cpsr    @change to user mode
	orr r3,#0xd0	@setting IRQ and FIQ flags 
	bic r3,r3,#0x0f	@setting mode
	msr cpsr,r3     @storing new cpsr
	 

	ldr sp,=0xa3000000
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
	
	ldr pc,=0xa2000000
	
	
