/*************************************************************
*
*Group Number : 40
*Group Members: Wahajuddin Syed <wsyed>
* 	        Jagannathan Chakravarthy <jchakrav>
*
**************************************************************/

/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>
#include <glob.h>


unsigned long time_syscall(void)
{
 return(tot_time);/* remove this line after adding your code here */	
}



/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep.
 *
 * 
 */
void sleep_syscall(unsigned long millis  __attribute__((unused)))
{
unsigned int temp;
temp=tot_time + millis; //calculate wait limit
while(tot_time<temp);	//stall till limit
return(1);	
}
