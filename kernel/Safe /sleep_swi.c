//swi implementation of sleep
#include <exports.h>
#include <glob.h>

int sleep_swi(int waitms)
{
unsigned int temp;
temp=tot_time + waitms; //calculate wait limit
while(tot_time<temp);	//stall till limit
return(1);
}
