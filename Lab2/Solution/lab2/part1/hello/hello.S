@ hello.S: Hello world! in assembly
@
@ Authors: Satvik Dhandhania <sdhandha@andrew.cmu.edu>
@ Date:    10/10/2015    15:00pm
@ We first set the file descriptor to be that of 
@ STDOUT_FILENO that is 1 and store it in r0 to be passed. 
@ The program refers to the hello_str label to get the
@ "Hello World\n" string and passes it's address  to r1. 
@ The string size is calculated on runtime and passed 
@ to register to r2 which is followed by the call to 
@ the call to WRITE_SWI handler. Finally, EXIT_SWI is 
@ called with status returned as 0.
#include<bits/swi.h>

	.file	"hello.S"
	.text

	.global	_start
_start:
	mov r0, #1		
	ldr r1, =hello_str
	mov r2, #hello_size
	swi WRITE_SWI
	mov r0, #0
	swi EXIT_SWI

	.section .rodata
hello_str:
	.ascii "Hello World\n"
	.set hello_size, .-hello_str

