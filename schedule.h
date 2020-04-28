#ifndef _SCHEDULING_H_
#define _SCHEDULING_H_

#include "process.h"

#define FIFO    0
#define RR      1
#define SJF     2
#define PSJF    3

/* Return next process to run */
//int next_process(struct process *proc, int nproc, int policy);

//my schedule
int scheduling(PROCESS *proc, int n_process, int policy);




#endif