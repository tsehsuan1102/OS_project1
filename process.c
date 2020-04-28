#define _GNU_SOURCE

#include "process.h"
#include <stdio.h>


void read_process(PROCESS *p){
    // printf("@@\n");
    //name, ready time, exec time
    scanf("%s", p->name);
    scanf("%d", &(p->ready_time));
    scanf("%d", &(p->exec_time));
    return ;
}



void print_process(PROCESS *p){
    fprintf(stderr, "%s %d %d\n", p->name, p->ready_time, p->exec_time);
}

