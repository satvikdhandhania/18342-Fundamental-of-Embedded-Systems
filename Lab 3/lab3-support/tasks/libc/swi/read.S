/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 * @author Vallari Mehta <vallarim@andrew.cmu.edu>
 * @date   Wed, 05 Nov 2015 01:52:31 -0400
 *
 * @ We start by calling the SWI handler for READ, we then check
 * @ the return value and compare with 0. If it is less than 0
 * @ there was an error so we make the value of r0 positive by
 * @ subtracting it from 0. We then set the global variable errno
 * @ to the positive value by using the global label errno. We 
 * @ first load it then move the value of r0 to it to denote the
 * @ error.After this we move -1 to r0 and return back by moving lr to pc.
 * @If it returns a positive value we simply return by
 * @ moving the link register back to the pc.
 *  
 *  @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 *  @date 2008-10-29
 */

#include <asm.h>
#include<bits/swi.h>

	.file "read.S"
    .text 

    .global read

FUNC(read)
    swi READ_SWI
    cmp r0, #0
    blt L0
    mov pc,lr

L0:
    rsb r0,r0,#0
    ldr r1, =errno
    str r0,[r1]
    mov r0, #-1
    mov pc,lr
