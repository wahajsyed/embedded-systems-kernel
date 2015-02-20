ARM_OBJS := reg.o psr.o int_asm.o irq_handler.o
ARM_OBJS := $(ARM_OBJS:%=$(KDIR)/arm/%)

KOBJS += $(ARM_OBJS)
