/**
 * file: irq installer
 *
 * saves the instructions at the current irq installer 
 and overwrites with the location of our handler
 */

#include <exports.h>
#include "glob.h"
#define ldr_inst 0xe59ff000
#define ldr_offset 0xe51ff004

unsigned irq_inst(void *location,unsigned *vector) 
{				//location of the swi handler 0x08 &f_ptr
unsigned temp,test;
test=*vector;			// access ldr pc,[pc,offset] inst at 0x08
test=test & 0xffff000;		//eliminate the offset and compare instruction
if((test ^ ldr_inst)==1)	// test if the instruction is ldr
{	printf("The given instruction is not ldr\n");
	return(-1);
}
temp=*vector;			// read the location
temp=(temp & 0x00000fff);	// obtain offset from inst

temp=temp+8+(unsigned)vector;	// add pc + 8 to offset to obtain the location of swi address

unsigned *temp_ptr= (unsigned *)temp;	// create a pointer to swi addrs location

unsigned *cirq_ptr=(unsigned *)*temp_ptr; //create a pointer to swi handlr address

instirq1=*cirq_ptr;		//save the current 1st inst
*cirq_ptr= ldr_offset;		//replace first instruction of swi hndlr
cirq_ptr=cirq_ptr+0x01;		//access the next inst
instirq2=*cirq_ptr;		//save the next inst
*cirq_ptr=(unsigned)location;	//replace the 2nd inst

return(1);

}
