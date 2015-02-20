/*************************************************************
*
*Group Number : 40
*Group Members: Wahajuddin Syed <wsyed>
* 	        Jagannathan Chakravarthy <jchakrav>
*
**************************************************************/

/**
 * file:device.c
 *
 * Implements simulated devices.
 */

#include <types.h>
#include <assert.h>
#include <config.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include "sched/sched_i.h"

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{

unsigned int i; //initialize the sleep queues and the next match of devices
		for(i=0;i<NUM_DEVICES;i++)
		{
			devices[i].sleep_queue=&system_tcb[IDLE_PRIO];
			devices[i].next_match=dev_freq[i];
		}
	
}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev __attribute__((unused)))
{

	unsigned int cur_prio=get_cur_prio();
	system_tcb[cur_prio].sleep_queue=devices[dev].sleep_queue; //update the link of the current waiting task
	devices[dev].sleep_queue=&system_tcb[cur_prio];		 //update the device sleep queue
}


/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis __attribute__((unused))) //no interrupts
{
	unsigned int c_prio=get_cur_prio();//check the current priority tcb
	unsigned int i;
	unsigned int temp;
	disable_interrupts();

	for(i=0;i<NUM_DEVICES;i++)
	{
					
		if(millis==devices[i].next_match) //compare the match time to the device times
		{
		
			if((*devices[i].sleep_queue).cur_prio!=63)	//compare the queue prio to 63, 63 means empty queue
			{
				runqueue_add(devices[i].sleep_queue,(*devices[i].sleep_queue).cur_prio); //add the current task to run
				temp=(*devices[i].sleep_queue).cur_prio; //move to the next element
		
				while((*system_tcb[temp].sleep_queue).cur_prio!=63)		//check if the next tcb points to 63
				{
					runqueue_add(system_tcb[temp].sleep_queue,(*system_tcb[temp].sleep_queue).cur_prio); //add the tcb to que
					(*system_tcb[temp].sleep_queue).sleep_queue=&system_tcb[IDLE_PRIO];
					temp=(*system_tcb[temp].sleep_queue).cur_prio;	
				}
				devices[i].sleep_queue=&system_tcb[IDLE_PRIO]; //device pointer updated back to null
			}
			devices[i].next_match+=dev_freq[i];	//next match updated by dev freq
		
		}

	}	
	if(highest_prio()<c_prio)	
		{
		dispatch_save();
		}		
	enable_interrupts();				
}

