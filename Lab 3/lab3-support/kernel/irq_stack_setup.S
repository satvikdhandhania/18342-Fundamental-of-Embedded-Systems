@ user_mode_setup.S: User mode startup routine
@
@ Author: Satvik Dhandhania < sdhandha@andrew.cmu.edu >
@         Vallari Mehta < vallarim@andrew.cmu.edu >
@ Date:   Tue, 23 Oct 2015 11:20:33

#define IRQ_STACK_BEGINNING 0xa1000000
    .file	"irq_stack_setup.S"
	.text

	.global	irq_stack_setup

irq_stack_setup:

    stmfd sp!,{r0-r12,lr}	@storing registers to stack
	
	mrs r3, cpsr    @change to irq mode
	bic r3,r3,#0xf	@clearing mode bits 
	orr r3,#0x82	@setting mode and disabling IRQ
	msr cpsr,r3     @storing new cpsr

	ldr sp,=IRQ_STACK_BEGINNING
	
    mrs r3, cpsr    
	orr r3,#0x3	    @ changing to SVC mode 
	bic r3,r3,#0x80 @enabling irq interrupts
    msr cpsr,r3     @storing new cpsr
    ldmfd sp!,{r0-r12,lr}
    mov pc, lr 
