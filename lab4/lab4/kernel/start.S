/**
 * @file  start.S
 *
 * @brief Kernel startup routine
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Tue, 23 Oct 2007 11:20:33 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-07-06
 */

#include <asm.h>

	.file   "start.S"

FUNC(_start)
	/*
	 * Branch immediately to main without altering the execution environment.
	 * This indirection is needed to guarantee that the program entry point is
	 * LOAD_ADDR (probably 0xa3000000) without placing strict
	 * requirements on the layout of kernel.c.  Bit first, provide main with the
	 * jump table address as one of its arguments.
	 */

/*
    Passing register r8, sp and link register to the kernel main function.
    Saving current CPSR to restore at the end */
    stmfd sp!, {r4-r12,lr}

    @Saving CPSR in global variable
    ldr r3,=CPSR
    mrs r2,cpsr
    str r2,[r3]

    mov     r2, r8
    mov     r3, sp
    str     lr, [sp]
    b       kmain


