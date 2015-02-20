//swi implementation of read
#include <exports.h>
#define memory_upper 0xa3ffffff
#define memory_lower 0xa0000000
int read_swi(int fd,char *buff,int count) 
{
	int i,j=0;
	unsigned int range;
	char temp;
	char back[]="\b \b";		//backspace string init
	range =(int)buff + count;
		if(fd!=0)		//check file descriptor
		{
		return(-9);		//errno value for EBADF is 9
		}
	 	if(range>memory_upper || range<memory_lower)
		{
		return(-14);		//memory fault return
		}		
	for(i=0;i<count;i++)		//count down
		{
		while(tstc()==0);	//stalls till valid data bit is set prevents getc blocking
		temp=getc();		//store char in temp 
		if(temp==4)		//check EOT
			{
			buff[j]=00; //null character
			return(j);
			}	 	//return the size read
		else if(temp==8 || temp==127) //check backspace
			{
			puts(back);
			j=j-1;i=i-2;	//decrement size
			} 		
		else 
			{
			putc(temp);	//display written char
			buff[j]=temp;	//write char to buffer
			j=j+1;		//increase size
			if(temp==10 || temp==13)//newline
				{
				buff[j]=00;//add null
				puts(buff);
				puts("\n");
				return(j);
				}
			}
		}

	buff[j]=00;//add null
	puts("\n");
	return j;

}
