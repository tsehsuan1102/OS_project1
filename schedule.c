#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>

#include "process.h"
#include "schedule.h"
#include "mysort.c"

// #include "FIFO.c"
// #include "RR.c"
// #include "SJF.c"
// #include "PSJF.c"


static  int     timer;      //now time
static  int     now_run_id; //now running process
static  int     finished;   //count finished processes
static  int     t_last;   



void run_FIFO(PROCESS **process_list, int n_process, int type)
{
    //TODO sort
    //sort by time
    mysort(process_list, n_process);
}


//find the next process
int next_process(PROCESS **process_list, int n_process, int type)
{
//     if(type==FIFO){
//         if(now_run_id != -1)
//             return now_run_id;
//         else return -1;
//         for(int i=0;i<n_process;i++){
//             if(process_list[i]->exec_time > 0)
//                 return i;
//         }
//     }


    if(type==FIFO){
        //someone is running
        if(now_run_id!=-1)
            return now_run_id;

        int re = -1;
        for(int i=0; i<n_process;i++){
            if(process_list[i]->pid == -1 || process_list[i]->exec_time == 0)
                continue;
            if(re == -1 || process_list[i]->ready_time < process_list[re]->ready_time)
                re = i;
        }



    }
    
    if(type==RR){


    }


    if(type==SJF){
        if(now_run_id!=-1)
            return now_run_id;

        int re = -1;
        for(int i=0;i<n_process;i++){
            if( process_list[i]->pid == -1 || process_list[i]->exec_time == 0 )
                continue;
            // choose shortest one
            if( re == -1 || process_list[i]->exec_time < process_list[re]->exec_time )
                re = i;
        }

    }

    if(type==PSJF){
        int re = -1;
        for(int i=0;i<n_process;i++){
            if( process_list[i]->pid == -1 || process_list[i]->exec_time == 0 )
                continue;
            // choose shortest one
            if( re == -1 || process_list[i]->exec_time < process_list[re]->exec_time )
                re = i;
        }

    }


    /* Non-preemptive */
    // if(now_run_id!=-1 && (type==SJF || type==FIFO))
    //     return now_run_id;

    int re = -1;
    if(type == PSJF || type ==  SJF) {
        for(int i=0;i<n_process;i++)
        {
            if( process_list[i]->pid == -1 || process_list[i]->exec_time == 0 )
                continue;
            if( re == -1 || process_list[i]->exec_time < process_list[re]->exec_time )
                re = i;
        }
    }

    else if (type == FIFO) {
        for(int i = 0; i < n_process; i++) {
            if(process_list[i]->pid == -1 || process_list[i]->exec_time == 0)
                continue;
            if(re == -1 || process_list[i]->ready_time < process_list[re]->ready_time)
                re = i;
        }
    }

    else if (type == RR) {
        if (now_run_id == -1) {
            for (int i = 0; i < n_process; i++) {
                if (process_list[i]->pid != -1 && process_list[i]->exec_time > 0){
                    re = i;
                    break;
                }
            }
        }
        else if ((timer - t_last) % 500 == 0)  {
            re = (now_run_id + 1) % n_process;
            while (process_list[re]->pid == -1 || process_list[re]->exec_time    == 0)
                re = (re + 1) % n_process;
        }
        else
            re = now_run_id;
    }

    return re;
}



int scheduling(PROCESS **process_list, int n_process, int type)
{
    for (int i = 0; i < n_process; i++)
        process_list[i]->pid = -1;
    

    int     scheduler_pid = getpid();
    /* Set single core prevent from preemption */
    assign_proc_on_cpu(scheduler_pid, PARENT_CPU);

    /* Set high priority to scheduler */
    set_process_high(scheduler_pid);
    printf("%d set done", scheduler_pid);




    //FIFO
    if(type==FIFO){
        run_FIFO(process_list, n_process, type);
    }



    //init schedule_setting
    timer = 0;
    now_run_id = -1;
    finished = 0;

    while(1){ //in schedule
        // fprintf(stderr, "current time: %d\n", timer);
        // check whether running process is finished.
        // fprintf(stderr, "%d\n", now_run_id);
        if(now_run_id != -1 && process_list[now_run_id]->exec_time == 0){
            //finished
            //wait this process
            waitpid(process_list[now_run_id]->pid, NULL, 0);
            
            printf("%s: %d finished at time: %d\n", process_list[now_run_id]->name, process_list[now_run_id]->pid, timer);
            //no one running now.
            now_run_id = -1;
            finished++;
            if( finished==n_process )
            {
                fprintf(stderr, "all finished!\n");
                break;
            }
        }

// fprintf(stderr, "two\n");
        

        // if ready time, fork()
        for(int i=0;i<n_process;i++){
            // printf("%d\n", process_list[i]->ready_time);
            if(process_list[i]->ready_time == timer){
                fprintf(stderr, "fork %s\n", process_list[i]->name);
                process_list[i]->pid = run_process(process_list[i]);
                // print("")
                set_process_block(process_list[i]->pid);
            }
        }


// fprintf(stderr, "three\n");
        


        // get next process
        int next = next_process(process_list, n_process, type);
        // printf("~next:%d~\n", next);
        if(next!=-1) {
            // printf("next: %d\n", next);
            if(now_run_id != next){
                //higher next pid, lower now pid.
                set_process_high(process_list[next]->pid);
                if(now_run_id!=-1)
                    set_process_block(process_list[now_run_id]->pid);
                now_run_id = next;
                t_last = timer;
            }
        }




// fprintf(stderr, "four\n");
        


        //run a unit time.
        if (now_run_id != -1)
            process_list[now_run_id]->exec_time--;
        timer++;
        UNIT_TIME();
    }


    return 0;
}




