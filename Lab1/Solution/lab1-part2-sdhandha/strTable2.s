	.file	"part1-strTable.c"
	.text
	.align	2
	.global	strTable
	.type	strTable, %function
strTable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r6}
	add 	r1, r1, r3
	cmp 	r3, #0	
	beq	.L11
.L9:
	sub 	r6, r3, r3, LSL #1
	ldrb	ip, [r1, r6]	@ zero_extendqisi2
	mov 	r6, ip
.L7:
	sub	ip, ip, #23
	cmp	ip, #22
	bgt	.L7
.L13:
	cmp	ip, r2
	bgt	.L4
	strb	r6, [r0, ip]
.L4:
	subs   	r3, r3, #1
	bne	.L9
.L11:
	ldmfd	sp!, {r6}
	mov pc, lr
	.size	strTable, .-strTable
	.ident	"GCC: (GNU) 3.4.5"
