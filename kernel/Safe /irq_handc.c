#include <exports.h>
#include <arm/timer.h>
#include <arm/interrupt.h>
#include <arm/reg.h>
#include <glob.h>

void irq_handc(void)
{
reg_write(OSTMR_OSCR_ADDR,0);	//set the timer back to 0
tot_time=tot_time+10;		//increment the tot_time
reg_set(OSTMR_OSSR_ADDR,OSTMR_OSSR_M0); //acknowledges set status flag on status register
}
