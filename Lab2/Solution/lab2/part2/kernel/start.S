@ start.S: Kernel startup routine
@
@ Author: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
@ Date:   Tue, 23 Oct 2015 11:20:33

	.file	"start.S"
	.text

	.global	_start
_start:
	@ Branch immediately to main without altering the execution environment.
	@ This indirection is needed to guarantee that the program entry point is
	@ LOAD_ADDR (probably 0xa3000000) without placing strict
	@ requirements on the layout of kernel.c.
	
	@storing registers in global variable
	ldr r4,=stack_pointer
	str sp,[r4]
	ldr r4,=link_register
	str lr,[r4]
	ldr r4,=r_eight
	str r8,[r4]
	
	@Now implemented in kernel.c
	@ldr r4,=0x5c0009c0    @Address of 1st instruction
	@ldr r5,=0xe51fd980    @Instruction @1st line   need to save & restore
	@ldr r6,=0xe24dda41    @Instruction @2nd line   need to save & restore
	@stmfd sp!,{r5,r6}
	@ldr r7,=0xe51ff004    @New Instruction LDR pc,[pc,#-4]
	@ldr r2,=swihandler    @Loading address of swihandler
	@str r7,[r4]	      @Overwriting 1st instruction
	@str r2,[r4,#4]	      @Overwriting 2nd address with new handler instr
	
	b   main	      @Calling kernal.c main function	
