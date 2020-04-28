#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <stdbool.h>

// #include "header.h"
#include "process.h"
#include "schedule.h"



int check_type(char *str1)
{
    const char scheduling_types[5][5] = {"FIFO", "RR", "SJF", "PSJF"};
    for(int i=0;i<4;i++)
        if( strcmp(str1, scheduling_types[i]) == 0){
            return i;
        }
    return -1;
}


int main()
{
    char scheduling_policy[10];
    scanf("%s", scheduling_policy);
    int     type = check_type(scheduling_policy);
    // printf("### scheduling type = [%d]\n", type);
    // USE_CPU(getpid(), 0);
    // SET_PRIORITY(getpid(), SCHED_FIFO, 0);
    int     n_process;
    scanf("%d", &n_process);

    PROCESS *processes = (PROCESS*)malloc(n_process*sizeof(PROCESS));    
    PROCESS **process_list = (PROCESS**)malloc(n_process*sizeof(PROCESS*));
    for(int i=0;i<n_process;i++)
        process_list[i] = &processes[i];

    for(int i=0;i<n_process;i++){
        // print_process(process_list[i]);
        read_process(process_list[i]);
        // print_process(process_list[i]);
    }
    // fprintf(stderr, "#### Read Complete!\n");
    
    // for(int i=0;i<n_process;i++){
    //     print_process(process_list[i]);
    // }
    
    // fprintf(stderr, "#### Start schedule!\n");
    scheduling(process_list, n_process, type);




}






































