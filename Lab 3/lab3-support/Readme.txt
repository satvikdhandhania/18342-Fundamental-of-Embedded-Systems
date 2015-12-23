In start we are branching immediately to main without altering the execution environment, This indirection is needed to guarantee that the program entry point is LOAD_ADDR (probably 0xa3000000) without placing strict requirements on the layout of kernel.c.  Bit first, provide main with the jump table address as one of its arguments. Passing register r8, sp and link register to the kernel main function. Saving current CPSR to restore at the end.

In main we are calling two functions, initially hijacking Uboot's SWI handler with our custom handler, by sending SWI's base vector and mode to function hijack in which we are storing old instructions and replacing them with our new instruction.
Same is done for IRQ handler by sending IRQ Vector address as 0x18 and mode=2.

Now, we are setting irq stack by storing lr and register values on stack, loading sp value with beginning of stack, and then changing to SVC mode, also enabling IRQ interrupts and loading the registers and lr back.

We are then setting up time registers, by first saving OS match register, OS interrupt enable register, Interrupt Controller Level and Mask register, then clearing OSSR register for match register 0, enabling OIER for match register 0, Clearing ICLR register and setting ICMR register to generate IRQ's, count register is restarted and timer precision values are set.

Next, User stack is initialized, first registers and lr value are stored on stack, then mode is switched to user mode, argv and argc are stored on stack.

In our Swi handler, we are storing user mode register on stack, extracting swi number and based on swi number calling read, write, exit, time or sleep functions. In time, we are returning timer count value. For sleep, the sleep stays in loop until target time is reached after which, read and write are reading from and writing to command prompt respectively. After this, user registers are restored back.

In our irq handler, user mode registers and lr are stored on stack, note we do not need to disable further interrupts as they are automatically disabled by the processor while calling. To enable nesting we need to, enable it manually. Timer count is incremented and user registers are restored back from stack.

While exiting, we are restoring old swi and irq instructions, and restoring stack pointer, r8 and link registers.

Typo is a user application that chos characters back with timing data, we are calculating the start and end time of user inputting data and then finding difference, which is then written to command prompt.

Splat is a simple user application that displays spinning cursor and calls sleep for 200ms in between chars. 

Test is a user application built by us which initializes alarm clock, take time in seconds from user, sleep till the time and send a wake up call to user after target time is reached.

Interrupts are blocked during initializing stack and supervisor phase, r8, stack pointer and link register are stored globally and restored back, timer count is stored globally. 
We have selected irq stack to be at 0xa1000000, which is half way in space allocated for user stack and program, this is chosen because our application program reach this address after being loaded and irqs are not nested so stack will not grow further down, this may be changed to any place considered safe. 



