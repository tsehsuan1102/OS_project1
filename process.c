#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include "process.h"




void read_process(PROCESS *p){
    // printf("@@\n");
    //name, ready time, exec time
    scanf("%s", p->name);
    scanf("%d", &(p->ready_time));
    scanf("%d", &(p->exec_time));
    return ;
}


void print_process(PROCESS *p){
    //fprintf(stderr, "%s %d %d\n", p->name, p->ready_time, p->exec_time);
}


int assign_proc_on_cpu(pid_t pid, int core)
{
	fprintf(stderr, "pid %d\n", pid);
    if(core > 2){
        fprintf(stderr, "Core index error.");
        return -1;
    }

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(core, &mask);
    
    if( sched_setaffinity(pid, sizeof(mask), &mask) < 0 ) {
        perror("sched_setaffinity");
        exit(1);
    }
    // printf("[pid, core %d %d]\n", pid, core);
    return 0;
}


int set_process_high(int pid)
{
    struct sched_param param;
    param.sched_priority = 99;

    //SCHED_OTHER
    int ret = sched_setscheduler(pid, SCHED_OTHER, &param);
    if (ret < 0) {
        perror("sched_setscheduler");
        return -1;
    }
    return ret;
}


//run process
int run_process(PROCESS *now)
{
    int   pid = fork();

    if(pid<0){
        perror("fork");
        return -1;
    }
    //children
    if(pid==0){
        long    start, end;

		while( sched_getcpu()!=CHILD_CPU );

        start = syscall(GETTIME);
        for (int i=0;i<now->exec_time;i++) {
            //execute process
    		struct sched_param param;
            //while(param.sched_priority == 1){
			//	fprintf(stderr, "%d is halt...", pid);
			//	continue;
			//}
			UNIT_TIME();
        }
        end = syscall(GETTIME);
        
		syscall(PRINTK, getpid(), start, end);

        exit(0);
    }
    fprintf(stdout, "%s %d\n", now->name, pid);
	fflush(stdout);
    //parent
    //set children process on children CPU
    //assign_proc_on_cpu(pid, CHILD_CPU);
    
	return pid;
}

//set low priority
int set_process_block(int pid)
{
    struct sched_param param;
    param.sched_priority = 1;

    //SCHED_OTHER
    int ret = sched_setscheduler(pid, SCHED_IDLE, &param);
    if (ret < 0) {
        perror("sched_setscheduler");
        return -1;
    }
    return ret;
}





