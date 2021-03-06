/** @file proc.c
 * 
 * Implementation of `process' syscalls
 *
 * @author Wahajuddin Syed
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>
#include "../sched/sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
disable_interrupts();//Disable the interrupts

//printf("task create\n");

//check for schedulability
	unsigned int i,j,min,task_no,temp_prio;
	task_t temp;

//check for user program function location, data pointer, stack position, non negative time period, device time period, number of tasks
	for(i=0;i<num_tasks;i++) 	
	{
		
		if((unsigned int)tasks < 0xa0000000 || (unsigned int)tasks > 0xa3000000)//0xa0000000 0xa3000000
		{
		return(-EFAULT);
		}	
		if((unsigned int)tasks[i].lambda<0xa0000000 || (unsigned int)tasks[i].lambda > 0xa3000000)//0xa0000000 0xa3000000
		{
		return(-EFAULT);
		}
		if((unsigned int)&tasks[i].data<0xa0000000 || (unsigned int)&tasks[i].data > 0xa3000000)//0xa0000000 0xa3000000
		{
		return(-EFAULT);
		}
		if((unsigned int)tasks[i].stack_pos<0xa1000000 || (unsigned int)tasks[i].stack_pos>0xa2ffff00)//0xa0000000 0xa3000000
		{
		return(-EFAULT);
		}	
		if(num_tasks>OS_MAX_TASKS)
		{
		printf("The number of tasks are greater than the limit\n");
		return(-EINVAL);
		}
	}
	for(i=0;i<num_tasks;i++) //read the task priority and sort them according to times
	{	
		min=tasks[i].T;
		task_no=i;
		for(j=i+1;j<num_tasks;j++)
		{
			if(tasks[j].T<=min)
			{
				min=tasks[j].T;task_no=j;			
			}
		}	
		temp=tasks[i];
		tasks[i]=tasks[task_no];
		tasks[task_no]=temp;
	}

	
//if(assign_schedule(&tasks,num_tasks)==1)
//{
	allocate_tasks(&tasks,num_tasks);//call allocate
//}
//else
//{
//	printf("Error:The tasks are not schedulable!\n");
//	while(1);
//}  
return(0);
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
	int i;
	unsigned int prio_cur=get_cur_prio();
	if(dev>=NUM_DEVICES)
	{
	printf("The device requested:%u \n",dev);
	return(-EINVAL);	
	}
	disable_interrupts();//Disable the interrupts
	dev_wait(dev); 		//add the completed task to the sleep queue of the said dev
	runqueue_remove(prio_cur); //in event wait
	dispatch_save();	//add the task to sleep queue
	return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);
	disable_interrupts();
	while(1);
}
