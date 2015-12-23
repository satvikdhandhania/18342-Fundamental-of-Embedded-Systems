@   SWI handler implementation, gets SWI_NUM and calls C_SWI_handler
@   Author : Satvik DHandhania <sdhandha@andrew.cmu.edu>   
@            Vallari Mehta <vallarim@andrew.cmu.edu>
@   Date : 4th Novemeber 2015
    .file	"swihandler.S"
	.text

	.global	swihandler

swihandler:
    @Making space for spsr
    sub sp,sp,#4

    stmfd sp!, {r0-r12,lr} @Store User mode registers on stack
    @restore r8 register
    ldr r9,=global_data @Restore r8
    ldr r8,[r9]
    
    mrs r2,spsr
    str r2,[sp,#56]

    @Enabling interrupts again for timer interrupts
    mrs r5,cpsr
    bic r5,r5,#0x80
    msr cpsr,r5
    
    mov r1, sp          @r1 now contains pointer to parameters on stack 

    ldr r0, [lr, #-4]       @ extract the SWI number
    bic r0,r0,#0xff000000   @ get SWI # in r0 by bit-masking 
    
    @Storing user's sp and lr
    stmfd sp,{sp,lr}^
    sub sp,sp,#8

    bl C_SWI_handler        @ go to handler
    
    @restoring user's sp and lr
    ldmfd sp,{sp,lr}^
    add sp,sp,#8
    @Restoring spsr
    ldr r2,[sp,#56]
    msr spsr,r2

    ldmfd sp!,{r0-r12,lr} @ unstack user's registers and lr_svc 
    
    add sp,sp,#4
    
    movs pc, lr             @ return from handler
