ARM_OBJS := reg.o psr.o int_asm.o swihandler.o user_mode_setup.o C_SWI_handler.o C_IRQ_handler.o 
ARM_OBJS := $(ARM_OBJS:%=$(KDIR)/arm/%)

KOBJS += $(ARM_OBJS)
