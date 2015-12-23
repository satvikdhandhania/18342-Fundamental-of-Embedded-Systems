@   IRQ occurence sends to IRQ handler
@   Author Satvik Dhandhania <sdhandha@andrew.cmu.edu>
@          Vallari Mehta <vallarim@andrew.cmu.edu>
@   Date : 5th November 2015
@
.file	"irqhandler.S"
	.text

	.global	irqhandler

irqhandler:

	stmfd sp!, {r0-r12, lr} @Store User mode registers on stack

	@We do not need to disable further interrupts as they are automatically
    @disabled by the processor while calling. To enable nesting we need to 
    @enable it manually 
    
    bl C_IRQ_handler        @ go to handler
	
    ldmfd sp!, {r0-r12, lr} @ unstack user's registers and lr_svc 
    subs pc, lr, #4             @ return from handler
