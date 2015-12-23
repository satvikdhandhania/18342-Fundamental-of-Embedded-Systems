/** @file crt0.S
 *
 * @brief Execution startup routine
 * @ Authors: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 *            Vallari Mehta <vallarim@andrew.cmu.edu>
 * @ Date:    11/2/2015 3:00pm
 * @ This file simply takes the input parameters and passes it to the  
 * @ main function in correct format. The argc parameter is passed to 
 * @ r0 register. And the beginning address for the argv is passed to
 * @ register r1. We then call the main function using branch with link.
 * @ When main executes and returns we finally call the exit function and
 * @ pass to it whatever value the main returns to us.*
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:51:29 -0400
 */

#include <asm.h>

	.file "crt0.S"
    .text

    .global _start

FUNC(_start)
    ldr r0, [sp]
    add r1, sp, #4
    bl main
    b exit
