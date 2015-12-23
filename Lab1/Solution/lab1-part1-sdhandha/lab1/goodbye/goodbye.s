	.file	"hello.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Hello world!\000"
	.text
	.align	2
	.global	main
	.type	main, %function
.LC1:
	.ascii	"Goodbye world!\000"
	.text
	.align	2
	.global	main
	.type	main, %function

main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	ldr	r0, .L3			@loads the register r0 with .L3 which points to .LC0 containing the string "Hello world!"
	bl	puts			@prints the string in r0 using puts
	ldr 	r0, .L5			@loads the register r0 with .L3 which points to .LC0 containing the string "Goodbye world!"	
        bl 	puts			@prints the string in r0 using puts
	mov	r3, #42	
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, lr}
	bx	lr
.L4:
	.align	2
.L3:
	.word	.LC0
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.2"
.L5:
	.word	.LC1
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.2"

