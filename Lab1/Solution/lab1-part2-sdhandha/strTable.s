	.file	"part1-strTable.c"
	.text
	.align	2
	.global	strTable
	.type	strTable, %function
strTable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r6}	@ Reduced variables as they were redundant 
	add 	r1, r1, r3	@ Adding length of dst string to use in count down loop
	cmp 	r3, #0		@ Checking whether it is of 0 length or not to start execution 
	beq	.L11		@ If it is zero go to end
.L9:
	sub 	r6, r3, r3, LSL #1	@r6 = r3-2r3, setting r6 as a negative offset
	ldrb	ip, [r1, r6]	@ zero_extendqisi2 ;ip stores character, r1 is dst string
	mov 	r6, ip			@ Copy for future use of ip instead of loading again
	subpls  ip, ip, #23		@ Assuming dst is always a capital letter
	subpls  ip, ip, #23		@ dst char max value is 90, min =65. We need to subtract
	subpls  ip, ip, #23		@ Maximum 3 times to get in range of 0-22.
	addmis  ip, ip, #23		@ If number turns negative we make it positive again
.L13:
	cmp	ip, r2			@ Compare characters of both strings
	bgt	.L4			@ If r2>ip jump to .L4
	strb	r6, [r0, ip]		@ If not store the value of r6 to src string, no comparison required here
.L4:
	subs   	r3, r3, #1		@ replaced 2 instructions cmp and sub with subs
	bne	.L9			@ replaced blt with bne due to count down loop
.L11:
	ldmfd	sp!, {r6}
	mov pc, lr
	.size	strTable, .-strTable
	.ident	"GCC: (GNU) 3.4.5"
