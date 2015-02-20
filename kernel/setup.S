#define irq_enable_clear #0x8f
#define fiq_disable_user #0x50
#define user_stack 0xa3000000
#define userprogram_addr 0xa0000000

	.file "setup.S"
	.text
	.global setup
setup:
	stmfd	sp!,{r4-r12,lr}	@save suprvs r4-12  on svc stack before entering usr	
	mrs	ip,cpsr		@get cpsr
	bic	ip,ip,irq_enable_clear	@clear the last mode bits and irq bit
	orr	ip,ip,fiq_disable_user	@ disable fiq and change to user
	msr	cpsr,ip		@change mode
	ldr	sp,=user_stack	@setup user stack
	stmfd	sp!,{r0,r1}	@store src srgv on stack
	ldr	ip,=userprogram_addr	@store usr addr on ip	
	mov	pc,ip		@load pc from ip
