/** @file ctx_switch_asm.S
 *
 * @brief Contains the core context switch routine.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 * @author Kartik Subramanian
 * @date   2008-11-21
 */

	.file "ctx_switch_asm.S"

#include <asm.h>
#include <arm/psr.h>
#define ctx_size 40

/**
 * @brief Special exit routine from the scheduler that launches a task for the
 * first time.
 *
 * r4 contains the user entry point.
 * r5 contains the single argument to the user function called.
 * r6 contains the user-mode stack pointer.
 * Upon completion, we should be in user mode.
 */
FUNC(launch_task)
	mov     r0, r5
	mov     r1, #0
	mov     r2, #0
	mov     r3, #0
	mov     r5, #0
	mov     r8, #0
	mov     ip, #0
	msr     cpsr_c, #(PSR_MODE_USR)
	mov     sp, r6
	mov     r6, #0
	ldr     lr, =0xdeadbeef   /* Causes a crash instead of calling the reset vector */
	mov     pc, r4

/* r0 points to the target context, r1 to the current context. */
/* add your code to perform a full context switch */
FUNC(ctx_switch_full)//r0 next r1 current
	// tcb.context is the input
	//save current context and dispatch the said task
	//dispatch the said task by doing launch task remember registers
	
	add	r1,#ctx_size   //move to the end of the context structure	
	stmfd	r1,{r4-r11,sp,lr} //store the registers on the context
	b	ctx_switch_half


/*add your code to perform a half context switch */	
FUNC(ctx_switch_half)//r0,r1 r7 has the launch number r8 has the kernel stack position
	ldmfd	r0,{r4-r11,sp,lr} //load the registers
	mov	r1,#0		 // compare lr with ro(0)
	cmp	lr,r1		 //to check if the task has been launched before	
	bne	skip		 //if it has been launched before skip to skip
	b	launch_task	// else launch task
skip:
	mov	pc,lr
	
	
		
	

	//must be same but without the switch
