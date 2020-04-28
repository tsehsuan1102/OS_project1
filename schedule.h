#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#include "process.h"

#define FIFO    0
#define RR      1
#define SJF     2
#define PSJF    3

/* Return next process to run */
//int next_process(struct process *proc, int nproc, int policy);

//my schedule
int scheduling(PROCESS **process_list, int n_process, int policy);




#endif