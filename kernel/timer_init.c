/**
 * file :timer_inti.c
 * initialize the timer to 0 as soon as the kernel 
 * boots in order to keep better time
 */
 #include <exports.h>
#include <arm/timer.h>
#include <arm/interrupt.h>
#include <arm/reg.h>
#include <glob.h>
#define osmr0set 0x4000000
#define ten_ms 32500

void timer_init()
{
reg_write(OSTMR_OSCR_ADDR,0);		//write 0 in oscr
tot_time=0;				//initialize global variable
reg_write(OSTMR_OSMR_ADDR(0),ten_ms); 	//set the match to 10 ms
reg_write(OSTMR_OIER_ADDR,OSTMR_OIER_E0);//enable the match 0 interrupt
reg_set(INT_ICMR_ADDR,osmr0set);	//set icmr
reg_clear(INT_ICLR_ADDR,osmr0set);	//clear iclr	
}
