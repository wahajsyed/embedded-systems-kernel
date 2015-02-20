	.file "timer_inita.S"
	.text
	.global timer_inita
timer_inita:
	stmfd	sp!,{r0-r12,lr} @Store the interrupted programs register
	
	mrs	ip,cpsr		@get cpsr
	bic	ip,ip,#0x8f	@clear bits
	orr	ip,ip,#0x52	@set irq
	msr	cpsr,ip		@change mode
	ldr	sp,=0xa2000000	@setup irq stack	
	
	mrs	ip,cpsr		@get cpsr
	bic	ip,ip,#0x8f	@clear bits again
	orr	ip,ip,#0x53	@set svc mode
	msr	cpsr,ip		@change mode back to svc

	mrs	r0,cpsr		@get cpsr in ip
	bic	r0,r0,#0x80	@clear irq to enable interrupts
	msr	cpsr,r0		@store back into cpsr				
	bl	timer_init 	@branch to timer initialization c program
	ldmfd	sp!,{r0-r12,lr} @Load back the interrupted programs register	
	mov	pc,lr
