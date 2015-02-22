/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Wahajuddin Syed
 */

//#define DEBUG 0
#include <task.h>
#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

double s_array[64]={1.000000,0.828427,0.779763,0.756828,0.743492,0.734772,0.728627,0.724062,0.720538,0.717735,0.715452,0.713557,0.711959,0.710593,0.709412,0.708381,0.707472,0.706666,0.705946,0.705298,0.704713,0.704182,0.703698,0.703254,0.702846,0.702469,0.702121,0.701798,0.701497,0.701217,0.700955,0.700709,0.700478,0.700261,0.700056,0.699863,0.699681,0.699508,0.699343,0.699188,0.699040,0.698898,0.698764,0.698636,0.698513,0.698396,0.698284,0.698176,0.698073,0.697974,0.697879,0.697788,0.697700,0.697615,0.697533,0.697455,0.697379,0.697306,0.697235,0.697166,0.697100,0.697036,0.696974,0.696914};
 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	task_t *ptr= *tasks; //can directly access with prt[]
	int i;
	double sum=0;
	for(i=0;i<num_tasks-1;i++)
	{
	sum=sum+(double)(ptr[i].C/ptr[i].T);
	}
	sum=sum+(double)((ptr[i].C+ptr[i].B)/ptr[i].T);
	
	if(sum<=s_array[num_tasks-1])	
	{return 1;} 	
	else
	{return 0;}

}
	


