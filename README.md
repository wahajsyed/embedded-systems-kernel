# embedded-systems-kernel
Pieces of c and assembly code for the course project.

The kernel is a real time kernel capable of scheduling upto 64 tasks
Files:

device.c:
The tasks are associated with devices. As different devices have different 
trigger times and can have multiple tasks associated with the device, we 
maintain a global structure called dev, which keeps track of the sleep queue 
of the device and stores the next time when the device and the tasks associated
 with the devices are to be executed.
functions:

dev_init - Initializes the sleep queues to idle tasks and sets the initial 
match values for all devices. It is called 

dev_wait - Puts a task to sleep on top of the sleep queue until the next event is
 signalled for the device. Called by the event wait syscall

dev_update -Signals the occurrence of an event on all applicable devices. 
This function should be called on timer interrupts to determine that 
the interrupt corresponds to the event frequency of a device. If the 
interrupt corresponded to the interrupt frequency of a device, this 
function should ensure that all the tasks waiting on the matched device are 
made ready to run 

glob.c:
Holds the global variables used for storing overwritten instructions

irq_inst.c:
Saves the instructions at the current irq installer and overwrites them 
with the location of our interrupt handler.

main.c: The main function does the following tasks in the particular order-
Initializes the timer
-Obtains the address of our custom swi handler and calls the swi installer
-Obtains the address of our custom irq handler and calls the irq installer
-Initializes the mutex data structures
-Calls the setup function which sets the user stack and changes mode to user
and tranfers control to the user application
-If the user application is of return type, the main then restores the 
original interrupt and swi handlers and exits	

setup.S: The setup changes the mode from the superviser to user mode, it 
is responsible for setting up the user stack and loading the user program 
address. It is written in assembly because we are required to gain access 
to the registers.

swi_handa.S:
This function is called by the system call wrapper whenever a swi instruction 
is encountered,this piece of assembly stores the registers of the user,
obtains the swi number, restores the uboot table and calls the swi handler c 
function.

swi_handc.c: Depending on the number the swi handc calls the appropriate c function 
and tests the return value for errors
 
swi_inst.c:
Saves the instructions at the current swi installer and overwrites them 
with the  a jump to the location of our custom swi handler.
 
timer_init.c:
Initializes the timer to 0, sets the match value for 10ms, sets the timer 
interrput value.

timer_inita.S:
Configures the irq stack using mode changes, enables the irq interrupts
and branches to the c program

syscall/io.c:
Has the system calls(read, write) implementations using the API provided by the 
U-Boot.

syscall/proc.c:
Has the process related system calls, like task create(), which checks for 
schedulability and other things and then initiates the process of launching 
those tasks. event_wait() is used by a process which wants to wait on a 
particular device and, this allows the kernel to put the process in the sleep 
queue.

syscall/time.c:
This has the system calls time and sleep, time syscall return the time that 
has elapsed since the kernel booted, sleep syscall is used by a process for 
busy waiting for a given amount of time.
 
sched/ctx_switch.c : This has the functions dispatch_save(), dispatch_nosave() 
which are used to switch to the highest priority tasks with or without saving 
context. get_cur_prio() is used to get the current highest priority task that 
is available.

sched/ctx_switch_asm.S : This code is implemented in assembly and allows us to 
switch the tasks by saving the registers and restoring the resgisters for a new task.

sched/run_queue.c : This is responsible for maintaining the runqueues, adding 
and removing tasks from the runqueues, checking the highest priority task 
available in the run queue.

lock/mutex.c:
mutex_init: Initializes the mutex struct to initial values.
mutex_lock: Checks if the mutex is availabe, if it is already bloacked, go to the end of 
the linked list and add the task to the queue and dispathes other tasks, as 
soon as the task returns another attempt for locking is issued and returns 
either an error or only when we get the lock.
mutex_unlock:Tests the mutex identifier for errors,updates the tcb, removes the task  from the sleep queue.
