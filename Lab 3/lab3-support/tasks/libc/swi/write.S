/** @file write.S
 *
 * @brief write sycall wrapper
 *
 *@ Authors: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 *@          Vallari Mehta <vallarim@andrew.cmu.edu>
 *@   Date:    11/3/2015 3:05pm
 *@ We do similar actions to this as we did for read.S. Call
 *@ the software interrupt and then compare retrun value with 0.
 *@ If it is negative then the error value should be made positive.
 *@ This positive value should then be stored in the global variable
 *@ errno. This is done by loading the address of the global variable 
 *@ to a register and then storing the positive value in it. The return 
 *@ value should then be made -1 so that the calling function can check
 *@ for error. If the value of return is positive we return it as it is by
 *@ moving the link register back to the program counter.
 * @date   Sun, 07 Oct 2007 01:53:46 -0400
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>
#include<bits/swi.h>

	.file "write.S"
    .text

    .global write

FUNC(write)
    swi WRITE_SWI
    cmp r0, #0
    blt L0
    mov pc, lr

L0: rsb r0, r0, #0      @Making the negative return value positive
    ldr r1, =errno
    str r0, [r1]
    mov r0, #-1
    mov pc, lr
