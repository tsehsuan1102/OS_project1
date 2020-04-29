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
    param.sched_priority = 0;

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
    pid_t   pid = fork();

    if(pid<0){
        perror("fork");
        return -1;
    }
    //children
    if(pid==0){
        long    start, end;
        struct  timeval    tv;
        struct  timezone   tz;


		while( sched_getcpu()!=CHILD_CPU );

        start = syscall(GETTIME);

        // gettimeofday(&tv, &tz);
        // start_sec = tv.tv_sec;
        // start_nsec = tv.tv_usec;

        for (int i=0;i<now->exec_time;i++) {
            //execute process
            UNIT_TIME();
        }

        // gettimeofday(&tv, &tz);
        // end_sec = tv.tv_sec;
        // end_nsec = tv.tv_usec;
        end = syscall(GETTIME);

        // sprintf(dmesg, "[project1]%s %d %lld.%09lld %lld.%09lld\n", now->name, getpid(), start_sec, start_nsec, end_sec, end_nsec);
        syscall(PRINTK, getpid(), start, end);

        //fprintf(stderr, "%s\n", dmesg);
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
int set_process_block(pid_t pid)
{
    struct sched_param param;
    param.sched_priority = 0;

    //SCHED_OTHER
    int ret = sched_setscheduler(pid, SCHED_IDLE, &param);
    if (ret < 0) {
        perror("sched_setscheduler");
        return -1;
    }
    return ret;
}





