#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>

void USE_CPU(int pid, int x){
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET((x), &mask);
    if(sched_setaffinity((pid), sizeof(mask), &mask) != 0) {
        fprintf(stderr, "[Error] use cpu");
    }
    printf("ok\n");
}


void SET_PRIORITY(int pid, int policy, int priority){
    /* policy: SCHED_FIFO, SCHED_RR, SCHED_BATCH */
    /* real time scheduler policy (require root permission): SCHED_FIFO, SCHED_RR */ \
    /* non-real time scheduler policy: SCHED_BATCH */
    struct sched_param param;
    param.sched_priority = priority;
    if (sched_setscheduler(pid, policy, &param) != 0){
        fprintf(stderr, "[ERROR] SET_PRIORITY\n");
    }
}



