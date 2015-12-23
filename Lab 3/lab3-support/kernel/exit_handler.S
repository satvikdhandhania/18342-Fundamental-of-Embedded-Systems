@ exit_handler.S: Kernel startup routine
@
@ Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
@         Vallari Mehta <vallarim@andrew.cmu.edu>  
@ Date:   Tue, 23 Oct 2015 11:20:33

	.file	"exit_handler.S"
	.text

	.global	exit_handler

exit_handler:

    stmfd sp!,{r0} 
    
	bl restore_timer

    ldmfd sp!,{r0}
	
	ldr r5,=swi_address         @Address to restore old swi handler
	ldr r4,[r5]
    
	ldr r3,=swi_instruction1	@Restoring SWI instruction 1
	ldr r5,[r3]
	str r5,[r4]

	ldr r3,=swi_instruction2    @Restoring SWI instruction 2
	ldr r5,[r3]
	str r5,[r4,#4]
	
    ldr r5,=irq_address         @Address to restore old irq handler
	ldr r4,[r5]

	ldr r3,=irq_instruction1	@Restoring IRQ instruction 1
	ldr r5,[r3]
	str r5,[r4]

	ldr r3,=irq_instruction2    @Restoring IRQ instruction 2
	ldr r5,[r3]
	str r5,[r4,#4]
    
    ldr r4,=CPSR            @Restoring cpsr register
    ldr r3,[r4]	
    msr cpsr,r3
    
	ldr r4,=stack_pointer   @Restoring sp from global variable
	ldr sp,[r4] 

    ldmfd sp!,{r4-r12,lr}
    mov pc,lr
