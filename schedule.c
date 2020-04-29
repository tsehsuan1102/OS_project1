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




typedef struct queue{
	int data[20];
	int front, back;
}Queue;

void init(Queue *que){
	que->front = 0;
	que->back = 0;
}
int size(Queue *que){
	return ((que->back) - (que->front)+20)%20;
}
void push(Queue *que, int x){
	que->data[que->back++] = x;
	que->back %= 20;
}
int top(Queue *que){
	//printf("!!%d\n", que->data[que->front]);
	int tmp = que->data[que->front];
	//printf("%d\n", tmp);	

	return tmp;
}
void pop(Queue *que){
	que->front++;
	que->front %= 20;
}







#define time_slice 500

void run_FIFO(PROCESS **process_list, int n_process, int type)
{
    //TODO sort
    //sort by time
    mysort(process_list, n_process);
}

Queue myque;

//find the next process
int next_process(PROCESS **process_list, int n_process, int type)
{
    int re = -1;

    //FIFO
    if(type==FIFO){
        //someone is running
        if(now_run_id!=-1)
            return now_run_id;
        //else
        for(int i=0; i<n_process;i++){
            //cannot be candidate
            if(process_list[i]->pid == -1 || process_list[i]->exec_time == 0)
                continue;
            // choose the minimun ready time
            if( re == -1 || process_list[i]->ready_time < process_list[re]->ready_time)
                re = i;
        }
    }


    //Round Robin
    if(type==RR){
		//queue	
        if(now_run_id==-1){
			if( size(&myque)>0 ){
				int nowid = top(&myque); pop(&myque);
				return nowid;
			}
			else return -1;
		}
		else if((timer - t_last) % time_slice == 0) {
            //re = (now_run_id+1)%n_process;
            //while (process_list[re]->pid == -1 || process_list[re]->exec_time == 0){
             //   re++;
             //   re %= n_process;
             //   if(re == now_run_id){
                    //a round
                    //if(process_list[re]->exec_time == 0)
                    //    re = -1;
			if( now_run_id!=-1 && process_list[now_run_id]->exec_time > 0 ){
				push(&myque, now_run_id);
			}

			if( size(&myque)>0 ){
				int nowid = top(&myque); pop(&myque);
				return nowid;
			}
			else{
				return -1;
			}
		}
		return now_run_id;
		/*
        //no one is running
        if(now_run_id==-1){
            for(int i=0;i<n_process;i++){
                if(process_list[i]->pid != -1 && process_list[i]->exec_time > 0){
                    re = i;
                    break;
                }
            }
        }
        //
        else if((timer - t_last) % time_slice == 0)  {
            re = (now_run_id+1)%n_process;
            while (process_list[re]->pid == -1 || process_list[re]->exec_time == 0){
                re++;
                re %= n_process;
                if(re == now_run_id){
                    //a round
                    //if(process_list[re]->exec_time == 0)
                    //    re = -1;
                    break;
                }
            }
        }
        else
            re = now_run_id;
		//fprintf(stderr, "!!%d\n", re);
		*/
    }


	
    if(type==SJF){
        //if someone is running, then cannot interrupt it.
        if(now_run_id!=-1)
            return now_run_id;

        // int tmp_ready = 10000000;
        for(int i=0;i<n_process;i++){
            if( process_list[i]->pid == -1 || process_list[i]->exec_time == 0 )
                continue;
            // choose shortest one
            if( re == -1 || (process_list[i]->exec_time < process_list[re]->exec_time || (process_list[i]->exec_time == process_list[re]->exec_time && process_list[i]->ready_time < process_list[re]->ready_time)) )
                    re = i;
        }

    }

    if(type==PSJF){
        for(int i=0;i<n_process;i++){
            if( process_list[i]->pid == -1 || process_list[i]->exec_time == 0 )
                continue;
            // choose shortest one
            if( re == -1 || process_list[i]->exec_time < process_list[re]->exec_time )
                re = i;
        }

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
    //printf("%d set done", scheduler_pid);

    //FIFO, sort at first
    if(type==FIFO){
        run_FIFO(process_list, n_process, type);
    }


    //init schedule_setting
    timer = 0;
    now_run_id = -1;
    finished = 0;
	init(&myque);
    
	while(1){ //in schedule

		//fprintf(stderr, "TIMER: %d\n", timer);

        // check whether running process is finished.
        if(now_run_id != -1 && process_list[now_run_id]->exec_time == 0){
            //wait this process
            waitpid(process_list[now_run_id]->pid, NULL, 0);
            
            fprintf(stderr, "%s: %d finished at time: %d\n", process_list[now_run_id]->name, process_list[now_run_id]->pid, timer);
            
			//no one running now.
            now_run_id = -1;
            finished++;
            if( finished==n_process )
            {
                // fprintf(stderr, "all finished!\n");
                break;
            }
        }

        
        // if ready time, fork()
        for(int i=0;i<n_process;i++){
            // printf("%d\n", process_list[i]->ready_time);
            if(process_list[i]->ready_time == timer && process_list[i]->pid==-1){
                //fprintf(stderr, "fork %s\n", process_list[i]->name);
                if(type==RR)
					push(&myque, i);
				process_list[i]->pid = run_process(process_list[i]);
                set_process_block(process_list[i]->pid);
				
            }
        }

        // get next process
        int next = next_process(process_list, n_process, type);
        //printf("~next:%d~\n", next);
        if(next!=-1) {
            //change work
            if(now_run_id != next){
                //higher next pid, lower now pid.
                set_process_high(process_list[next]->pid);
				//fprintf(stderr, "set!\n");
				assign_proc_on_cpu(process_list[next]->pid, CHILD_CPU);
				
                if(now_run_id!=-1)
                    set_process_block(process_list[now_run_id]->pid);
                now_run_id = next;
                t_last = timer;
            }
        }

        //run a unit time.
        if (now_run_id != -1)
            process_list[now_run_id]->exec_time--;
        timer++;
        UNIT_TIME();
    }


    return 0;
}






