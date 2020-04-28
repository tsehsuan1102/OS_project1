#include <stdbool.h>
#include <stdio.h>

#include "process.h"


bool cmp(PROCESS *a, PROCESS *b)
{
    if(a->ready_time > b->ready_time)
        return true;
    return false;
}

void mysort(PROCESS **process_list, int n_process)
{
    // for(int i=0;i<n_process;i++){
    //     print_process(process_list[i]);
    // }
    for(int r=n_process-1;r>=0;r--){
        for(int j=0;j<r;j++){
            if( cmp(process_list[j], process_list[j+1]) ){
                PROCESS* tmp = process_list[j];
                process_list[j] = process_list[j+1];
                process_list[j+1] = tmp;
            }

        }
    }
    // puts("!");
    for(int i=0;i<n_process;i++){
        print_process(process_list[i]);
    }
    

    return ;
}


