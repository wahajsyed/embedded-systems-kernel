//task create
//read the task priority
//call allocate/
#include <exports.h>
#include <task.h>
void allocate_tasks(task_t**, size_t);

int taskcreate_swi(task_t* taskp, size_t num_tasks)
{
printf("num tasks %d",num_tasks);
allocate_tasks(&taskp,num_tasks);
//read the task priority
//call allocate  
return(0);
} 
