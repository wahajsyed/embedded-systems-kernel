#include <bits/swi.h>
#define clear_modebits #0xf
#define svcmode_ifdisable #0xd3

	.file "irq_handa.S"
	.text
	.global irq_handa
irq_handa:
	stmfd	sp!,{r0-r12,lr} @Store the interrupted programs register

	mrs	ip,cpsr		@get cpsr
	bic	ip,ip,clear_modebits	@clear the last 4 bits
	orr	ip,ip,svcmode_ifdisable	@disable fiq,irq and change to supervisor
	msr	cpsr,ip		@change mode		

	bl	irq_handc 	@branch
	ldmfd	sp!,{r0-r12,lr} @Load back the interrupted programs register			
	movs	pc,lr		@go back to the interrupted program
	
