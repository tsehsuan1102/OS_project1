#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <sys/types.h>

#define PARENT_CPU  0
#define CHILD_CPU   1

//Unit time
#define UNIT_T(){ for(int i=0;i<1000000;i++);}


typedef struct{
    char    name[32];
    int     ready_time;
    int     exec_time;
    pid_t   pid;
}PROCESS;



void read_process(PROCESS *p);

void print_process(PROCESS *p);




// /* Assign process to specific core */
// int proc_assign_cpu(int pid, int core);

// /* Execute the process and return pid */
// int proc_exec(struct process proc);

// /* Set very low priority tp process */
// int proc_block(int pid);

// /* Set high priority to process */
// int proc_wakeup(int pid);

#endif