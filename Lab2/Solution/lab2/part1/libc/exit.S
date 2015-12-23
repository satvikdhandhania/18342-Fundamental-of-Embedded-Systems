@ exit.S: exit sycall wrapper
@
@ Authors: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
@ Date:    10/10/2015   15:00:00
@ 
@ For exit we do not care about setting the return status
@ as whatever will be passed to this will be directly passed 
@ to the SWI handler EXIT_SWI

#include<bits/swi.h>

	.file	"exit.S"
	.text

	.global	exit
exit:
	swi EXIT_SWI
