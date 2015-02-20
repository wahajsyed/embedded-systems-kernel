#include <exports.h>
#include <arm/timer.h>
#include <arm/interrupt.h>
#include <arm/reg.h>
#include <glob.h>
//report the global variable tot_time
unsigned long time_swi(void)
{
return(tot_time);
}
