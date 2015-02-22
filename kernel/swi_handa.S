#include <bits/swi.h>

#define only_swino  0xff000000
#define irq_enable 0x80

	.file "swi_handa.S"
	.text
	.global swi_handa
swi_handa:
	sub	sp,sp,#4	@make space for spsr
	stmfd	sp!,{r1-r12,lr}	@store registers at sp
	mrs	r4,spsr		@move spsr into r4
	str	r4, [sp,#13*4]	@store spsr in stack
@enabling irqs
	mrs	ip,cpsr		@get cpsr
	bic	ip,ip,#irq_enable@enable the irq
	msr	cpsr,ip		@save back the register

@Recover and save user registers because previously we saved svc registers
	mrs      r4, spsr
	stmfd    sp, {r4, sp, lr}^
	add      sp, sp, #-12

@obtaining swi number and exit service
	ldr	r3,[lr,#-4]	@access the swi instruction call
	bic	r3,r3,#only_swino @get the swi number in arg4
	ldr	r4,=CREATE_SWI	@store create value in r4
	cmp	r3,r4		@compare for create
	bne	_continue	@implement exit with local branch
	mov	r2,r1	
	mov	r1,r0

@Storing Uboot table	
_continue:
	ldr	r5,=global_data	@store the global data address in r4
	ldr	r8,[r5]		@restore the Uboot table fro r4
@Swi handler call and return
	bl	swi_handc	@call c swi handler for others

@Restore registers
	ldmfd    sp, {r4, sp, lr}^
	add      sp, sp, #12
	msr      spsr, r4

	ldr	r4, [sp,#13*4]	@load back spsr into r4
	msr	spsr,r4		@restore spsr
	ldmfd	sp!,{r1-r12,lr}	@restore registers
	add	sp,sp,#4	@remove spsr space
	movs	pc,lr		@branch back to calling program

			
