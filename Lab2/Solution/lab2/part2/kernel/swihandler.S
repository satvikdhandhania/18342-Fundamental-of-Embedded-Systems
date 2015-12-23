	.file	"swihandler.S"
	.text

	.global	swihandler

swihandler:

	stmfd sp!, {r0-r12, lr} @Store User mode registers on stack
	ldr r8, [sp,#56]
	mov r1, sp              @r1 now contains pointer to parameters on stack 
	ldr r0, [lr, #-4]       @ extract the SWI number
	bic r0,r0,#0xff000000   @ get SWI # in r0 by bit-masking 
	bl C_SWI_handler        @ go to handler
	ldmfd sp!, {r0-r12, lr} @ unstack user's registers and lr_svc 
	movs pc, lr             @ return from handler
