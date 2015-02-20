/*************************************************************
*
*Group Number : 40
*Group Members: Wahajuddin Syed <wsyed>
* 	        Jagannathan Chakravarthy <jchakrav>
*
**************************************************************/

#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <glob.h>

#define swi_addr 0x00000008
#define irq_addr 0x00000018

#define old_swi_add 0x5c0009c0
#define old_irq_add 0x5c000b40

int read_swi(int,char *,int);
int task_create(task_t* tasks, size_t num_tasks);

void timer_inita(void);
int swi_inst(void *,unsigned *);	//swi installer declaration
int irq_inst(void *,unsigned *);	//irq installer declaration

int swi_handa(int,char *,int,int);	//swi handler declaration
void irq_wrapper(void);
int setup(int,char **);			//user setup declaratoin
unsigned long time_swi(void); //to be removed

uint32_t global_data;



int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{
	app_startup();
	global_data = table;
	/* add your code up to assert statement */
	
timer_inita();				//Timer initialization
char buff[10];	
task_t tasks[2];
		
int y=0;
char c[255];				//char buffer
char *p;				//pointer to buffer
p=c;					//initialize pointer


printf("Timers and Interupts enabled\n");
//----------------------------SWI--------------------------- 
int (*f_ptrswi)(int,char *,int,int)= swi_handa; //obtain adress of swi handler
unsigned *vec= (unsigned *)swi_addr; 	// store vec address in vec
y=swi_inst(f_ptrswi,vec);		// call the installer
if(y!=1)				// check successful installation
	printf("Error while installing SWI\n");
else
	printf("SWI Install done at: %p \n",f_ptrswi);
//----------------------------IRQ----------------------------
void (*f_ptrirq)(void)= irq_wrapper; 	//obtain adress of irq handler
unsigned *vec2= (unsigned *)irq_addr; 	// store vec address in vec
y=irq_inst(f_ptrirq,vec2);		// call the installer
if(y!=1)				// check successful installation
	printf("Error while installing IRQ\n");
else
	printf("IRQ Install done at: %p \n",f_ptrirq);

mutex_init();
//--------------------------User----------------------------
printf("Starting execution of the User program...\n");
y=setup(argc, argv);			//user setup and transfer
//-------------------------Restoratoin---------------------

printf("Restoring Uboot SWI handler... \n");
unsigned *back= (unsigned *)old_swi_add;	//pointer to old swiaddress
*back=instswi1;					//restore first instruction
back=back+1;					//increment pointer
*back=instswi2;					//restore second instruction

printf("Restoring Uboot IRQ handler... \n");	
unsigned *back2= (unsigned *)old_irq_add;	//pointer to old swiaddress
*back2=instirq1;				//restore first instruction
back2=back2+1;					//increment pointer
*back2=instirq2;				//restore second instruction
	
assert(0);        /* should never get here */
}
