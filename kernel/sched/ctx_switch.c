/** @file ctx_switch.c
 * 
 * C wrappers around assembly context switch routines.
 *
 * @author Wahajuddin Syed
 */
 

#include <types.h>
#include <assert.h>
#include <task.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

unsigned int time_syscall();

static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */
/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
//update the current tcb
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
	//context switch full
	unsigned int prio_cur=(*cur_tcb).cur_prio;//get the current prio tcb
	unsigned int prio_next,i;
	prio_next=highest_prio();		//get the next highest prio
	cur_tcb=&system_tcb[prio_next];		//update the current tcb
	ctx_switch_full(&system_tcb[prio_next].context,&system_tcb[prio_cur].context);	

}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)//disable interrupts
{
					
	unsigned int prio=highest_prio();//switch to the highest priority task without saving current
	cur_tcb=&system_tcb[prio];	//update the current prio tcb
	ctx_switch_half(&system_tcb[prio].context);
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)//disable interrupts
{

}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	
	return (*cur_tcb).cur_prio; //fix this; dummy return to prevent compiler warning
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return cur_tcb; //fix this; dummy return to prevent compiler warning
}
