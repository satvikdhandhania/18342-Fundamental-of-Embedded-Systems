@ exit_handler.S: Kernel startup routine
@
@ Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
@ Date:   Tue, 23 Oct 2015 11:20:33

	.file	"exit_handler.S"
	.text

	.global	exit_handler

exit_handler:

	ldr r4,=stack_pointer   @Restoring sp from global variable
	ldr sp,[r4] 
	ldr r4,=link_register   @Restoring link register
	ldr lr,[r4] 
	ldr r4,=r_eight         @Restoring register r8
	str r8,[r4]
	
	ldr r4,=address         @Address to restore old swi handler

	ldr r3,=instruction1	@Restoring instruction 1
	ldr r5,[r3]
	str r5,[r4]

	ldr r3,=instruction2    @Restoring instruction 2
	ldr r5,[r3]
	str r5,[r4,#4]
	mov pc,lr
