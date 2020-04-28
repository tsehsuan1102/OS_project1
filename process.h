#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <sys/types.h>

#define PARENT_CPU  0
#define CHILD_CPU   1
#define GETTIME     333
#define PRINTK      334



//Unit time
#define UNIT_TIME() { volatile unsigned long i; for(i=0;i<1000000UL;i++); } 


typedef struct{
    char    name[50];
    int     ready_time;
    int     exec_time;
    pid_t   pid;
}PROCESS;



void    read_process(PROCESS *p);

void    print_process(PROCESS *p);

// Assign process to specific core
int     assign_proc_on_cpu(int pid, int core);

//run process
int     run_process(PROCESS *now);

//High priority
int     set_process_high(int pid);

//set low priority
int     set_process_block(int pid);

#endif