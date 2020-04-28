#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>

#include "process.h"
#include "scheduler.h"






int scheduling(PROCESS *process_list, int n_process, int type)
{
    //FIFO
    if(type==FIFO){


    }
    //RR
    if(type==RR){


    }
    //SJF
    if(type==SJF){


    }
    //PSJF
    if(type==PSJF){


    }



}




