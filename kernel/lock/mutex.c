/**
 * file: mutex.c
 *
 * brief Implements mutices.
 *
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include "../sched/sched_i.h"
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];

void mutex_init()
{
static int check_mutex=0;
int i;
	
		
		for(i=0;i<OS_NUM_MUTEX;i++) //initialize the pointer
			{
			if(check_mutex==0)
			{
			gtMutex[i].bAvailable=0;
			gtMutex[i].pHolding_Tcb=0;//pointer
			gtMutex[i].bLock=0;
			}
			gtMutex[i].pSleep_queue=&system_tcb[IDLE_PRIO];//pointer to idle task signifies empty queue
			}
		check_mutex=1;

}

int mutex_create(void)
{
	
	disable_interrupts();
	int i;
	for(i=0;i<OS_NUM_MUTEX;i++)
	{	
	if(gtMutex[i].bAvailable==0) //check if it is already created
		{
		gtMutex[i].bAvailable=1; //make it available
		enable_interrupts();
		return i;
		}
	}
	enable_interrupts();
	return	-ENOMEM; 
}

int mutex_lock(int mutex  __attribute__((unused)))
{
	disable_interrupts();
	int cur_prio,temp;
	if(mutex<OS_NUM_MUTEX)
	{
		if(gtMutex[mutex].bAvailable==0) //test if the mutex is available
		{
			enable_interrupts();
			printf("Requested mutex %u in not available: %u \n",mutex,gtMutex[mutex].bAvailable);			
			return(-EINVAL);
		}
		if(gtMutex[mutex].bLock==0) //test if it is block
		{
			gtMutex[mutex].bLock=1; //change to block
			cur_prio = get_cur_prio();	
			system_tcb[cur_prio].holds_lock = 1;			//assign the lock to the tcb
			gtMutex[mutex].pHolding_Tcb = &system_tcb[cur_prio];	//assign system_tcb of current task to holding tcb of mutex	
			enable_interrupts();		
			return 0;
			
		}
		else	
		{
			cur_prio = get_cur_prio();	
			if((*gtMutex[mutex].pHolding_Tcb).cur_prio == cur_prio)	//check if same task is asking for the 
			{	enable_interrupts();
				return -EDEADLOCK;			
			}
			else //implement the sleep queue
			{
				temp=(*gtMutex[mutex].pSleep_queue).cur_prio;
				
				if(temp==63) //queue was empty
				{
					gtMutex[mutex].pSleep_queue=&system_tcb[cur_prio];
					system_tcb[cur_prio].sleep_queue=&system_tcb[IDLE_PRIO];//add the tcb to sleep q
				}
				else //queue is not empty and we need to go to the end of the queue
				{
				while((*system_tcb[temp].sleep_queue).cur_prio!=63) //check if first element is last
					{
						temp=(*system_tcb[temp].sleep_queue).cur_prio;
					}
				system_tcb[temp].sleep_queue=&system_tcb[cur_prio];
				system_tcb[cur_prio].sleep_queue=&system_tcb[IDLE_PRIO];//add the tcb to sleep queue
				}
				runqueue_remove(cur_prio);//remove from runqueue,add to runqueue in unlock 				
				dispatch_save();	  //dispatch save , the program breaks from here
				mutex_lock(mutex);        //as soon as it comes back issue lock on the mutex
					
			}									
		}

	enable_interrupts();
	return mutex;
       	}	
	else
	{
	printf("Number not in the array\n"); 	
	enable_interrupts();
	return -EINVAL;
	}
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
	
	unsigned int c_prio = get_cur_prio();unsigned int temp;
	disable_interrupts();	
	if(gtMutex[mutex].bAvailable==0) //check availability
	{
		return(-EINVAL);
	}	
	if(mutex<OS_NUM_MUTEX)
	{
		if(gtMutex[mutex].bLock==0|| (*gtMutex[mutex].pHolding_Tcb).cur_prio!=c_prio)
		{
			enable_interrupts();
			return(-EPERM);
		}
		system_tcb[c_prio].holds_lock=0; //remove the lock by task
		gtMutex[mutex].bLock=0;		//make the mutex available
		gtMutex[mutex].pHolding_Tcb=&system_tcb[IDLE_PRIO]; //re initialize holding tcb
		temp=(*gtMutex[mutex].pSleep_queue).cur_prio;	    //checking the sleep que
		if(temp!=63)					    //check if sleep queue is full
		{
			runqueue_add(gtMutex[mutex].pSleep_queue,(*gtMutex[mutex].pSleep_queue).cur_prio); // remove the task from sleep queue
			gtMutex[mutex].pSleep_queue=(*gtMutex[mutex].pSleep_queue).sleep_queue;		 // update the sleep queue

		}
		
		enable_interrupts();
		return 0;
	}	
	printf("Requested mutex doesnt exist in the array\n");
	enable_interrupts();
	return -EINVAL;
	
}

