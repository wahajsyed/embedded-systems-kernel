/*************************************************************
*
*Group Number : 40
*Group Members: Wahajuddin Syed <wsyed>
* 	        Jagannathan Chakravarthy <jchakrav>
*
**************************************************************/

/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Wahajuddin Syed
 */

#include <types.h>
#include <assert.h>
#include <config.h>
#include <device.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#define i_stack 0xa3000000

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

void sched_init(task_t* main_task  __attribute__((unused)))
{

}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
       	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** task_ptr  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	unsigned int i;
	task_t *ptr= *task_ptr;
	static void (*idle_ptr)(void)=idle;
	task_t	task_idle;

	runqueue_init();

	for(i=0;i<num_tasks;i++)//Initializing the tcbs with the prios
	{
		system_tcb[i+1].cur_prio=i+1;
		system_tcb[i+1].native_prio=i+1;
		system_tcb[i+1].context.r4=(int)((ptr[i].lambda));
		system_tcb[i+1].context.r5=(int)((ptr[i].data));
		system_tcb[i+1].context.r6=(int)((ptr[i].stack_pos));
		system_tcb[i+1].context.lr=0;
		system_tcb[i+1].context.sp=(int)(&system_tcb[i+1].kstack_high[0]);
		runqueue_add(&system_tcb[i+1],i+1);
		//printf("the system task function %u is at %p\n",i,(ptr[i].lambda));	
		
	}
	/////////////allocate user stack and kernel context
	task_idle.lambda=idle; //idle
	task_idle.stack_pos=(void *)i_stack; // last possible stack value
	system_tcb[IDLE_PRIO].cur_prio=IDLE_PRIO;
	system_tcb[IDLE_PRIO].native_prio=IDLE_PRIO;
	system_tcb[IDLE_PRIO].context.r4=(int)((task_idle.lambda));
	system_tcb[IDLE_PRIO].context.r5=0xdeadbeef;
	system_tcb[IDLE_PRIO].context.r6=(int)((task_idle.stack_pos));
	system_tcb[IDLE_PRIO].context.lr=0;
	system_tcb[IDLE_PRIO].context.sp=(int)(&system_tcb[IDLE_PRIO].kstack_high[0]);
	runqueue_add(&system_tcb[IDLE_PRIO],IDLE_PRIO);

	
	//printf("Dispatching the first task with nosave\n");
	dev_init();//initializing the device structure
	mutex_init();	
	dispatch_nosave();//dispatch nosave first task
}

