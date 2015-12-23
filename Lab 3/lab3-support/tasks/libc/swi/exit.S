/** @file exit.S
 *
 * @brief exit sycall wrapper
 *
 * @author Satvik Dhandhania <sdhandha@andrew.cmu.edu>
 * @author Vallari Mehta <vallarim@andrew.cmu.edu>
 * @date   Wed, 05 Nov 2015 01:52:31 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>
#include<bits/swi.h>

	.file "exit.S"
    .text 

    .global exit

FUNC(exit)
    swi EXIT_SWI
