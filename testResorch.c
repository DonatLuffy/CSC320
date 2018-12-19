#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
// #include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include "DataStructures.h"
#include <sys/ipc.h>
#include <sys/msg.h>

int checkAvaliableResources(Resources);
Queue* JobsQueue;

int main( ) {
        Queue* JobsQueue=new_queue();
        Job job1,job2,job3,job4,job5;
        job1.number = 1;
        job2.number = 2;
        job3.number = 3;
        job4.number = 4;
        job5.number = 5;
        enqueue(JobsQueue, &job1);//add the job to the Queue
        enqueue(JobsQueue, &job2);//add the job to the Queue
        enqueue(JobsQueue, &job3);//add the job to the Queue
        enqueue(JobsQueue, &job4);//add the job to the Queue
        enqueue(JobsQueue, &job5);//add the job to the Queue


        for (int i = 0; i < 5; i++) {
                Job j=*((Job*)dequeue(JobsQueue));
                printf("dque %d\n",j.number );
        }

        return 0;
}

int checkAvaliableResources(Resources r){
        return ((r.resource_A && r.resource_B)&&(r.resource_C && r.resource_D));
}
