/*************************************************************
*
*Group Number : 40
*Group Members: Wahajuddin Syed <wsyed>
* 	        Jagannathan Chakravarthy <jchakrav>
*
**************************************************************/


/** file io.c
 *
 * Kernel I/O syscall implementations
 *
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f

#define mem_up 0xa3ffffff 
#define mem_down 0xa0000000


/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{

 	int i,j=0;
	unsigned int range;
	char temp;char * buff=(char *)buf;
	char back[]="\b \b";		//backspace string init
	range =(int)buff + count;
		if(fd!=0)		//check file descriptor
		{
		return(-EBADF);		//errno value for EBADF is 9
		}
	 	if(range>mem_up || range<mem_down)
		{
		return(-EFAULT);		//memory fault return
		}		
	for(i=0;i<count;i++)		//count down
		{
		while(tstc()==0);	//stalls till valid data bit is set prevents getc blocking
		temp=getc();		//store char in temp 
		if(temp==EOT_CHAR)		//check EOT
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

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const void *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{

	int i,j=0;
	char * buff=(char *)buf;
	unsigned int range;
	range =(unsigned int)buff + count;
		if(fd!=1)
		{
		return(-EBADF);	//errno value for EBADF is 9
		}
	 	if(range>mem_up || range<mem_down)
		{
		return(-EFAULT);	//memory fault return
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

