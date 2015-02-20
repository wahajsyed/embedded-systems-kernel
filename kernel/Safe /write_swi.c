//swi implementation of write
#include <exports.h>
#define mem_up 0xa3ffffff 
#define mem_down 0xa0000000
int write_swi(int fd,const char *buff,int count) 
{
	int i,j=0;
	unsigned int range;
	range =(unsigned int)buff + count;
		if(fd!=1)
		{
		return(-9);	//errno value for EBADF is 9
		}
	 	if(range>mem_up || range<mem_down)
		{
		return(-14);	//memory fault return
		}
	
		printf("%s",buff); //print string
		for(i=0;i<count;i++)	//check for new line
		{
			if(buff[i]==0)
			return(j);
		j=j+1;
		}
		return(j);
}
