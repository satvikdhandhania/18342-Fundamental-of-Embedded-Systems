SYSCALL_OBJS := proc.o time.o io.o
SYSCALL_OBJS := $(SYSCALL_OBJS:%=$(KDIR)/syscall/%)

KOBJS += $(SYSCALL_OBJS)
