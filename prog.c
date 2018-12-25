#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include "DataStructures.h"

/*functions prototype*/
void Generator();
void SchedulerJobs();
int Possibility();
int Random();
struct tm *getDate();
void handler();
void* Processor();
int checkAvaliableResources();
void reserveResources();

/* defined Constant*/
#define MAX_JOBS 100
int memory = 2000;
int PROCESSORS = 100;/*number of Processors avaliable*/
/*Intializes resources*/
Resources resourcesManager = {0,0,0,0};

/* declare Queue*/
Queue* JobsQueue;

/* set Handler*/
void handler(){
        exit(0);
}

void Generator(){
        while(1) {
                int i, id;
                int generates_maximum = Random(0,5);
                Job job;
                for (i = 0; i < generates_maximum; i++) {
                        if(id < MAX_JOBS) {
                                job.number = id++;
                                job.creation_date = getDate();
                                job.execution_time = Random(10,1000);
                                job.memory_requirement = Random(1,100);
                                job.resources.resource_A = Possibility();
                                job.resources.resource_B = Possibility();
                                job.resources.resource_C = Possibility();
                                job.resources.resource_D = Possibility();
                        }else{
                                if(id >= MAX_JOBS)
                                        exit(0);
                        }
                        //TODO write log here
                        /* Send to Scheduler Jobs */
                        SchedulerJobs(job);
                }
                sleep(1);
        }
}
void SchedulerJobs(Job job){
        enqueue(JobsQueue, &job);
        Job j = *((Job*)dequeue(JobsQueue));
        printf("ID: %d\n", j.number);
        if(checkAvaliableResources(j)) {/*Checks the resources requirement of the jobs*/
                reserveResources(j);
                if(PROCESSORS != 0) {/* dispatches the job to the first available processor */
                        PROCESSORS--;
                        pthread_t thread_id;
                        printf("Before Thread\n");
                        pthread_create(&thread_id, NULL, Processor, j);
                        pthread_join(thread_id, NULL);
                        printf("After Thread\n");
                        PROCESSORS++;
                }
                /* if PROCESSORS are busy */
                enqueue(JobsQueue, &j);
        }else{/* If resources are not available */
                printf("sleep until released\n");
                while(1) {
                        if(checkAvaliableResources(j))
                                break;
                        printf(".");
                }
                reserveResources(j);
                if(PROCESSORS != 0) {/* dispatches the job to the first available processor */
                        PROCESSORS--;
                        pthread_t thread_id;
                        printf("Before Thread\n");
                        pthread_create(&thread_id, NULL, Processor, j);
                        pthread_join(thread_id, NULL);
                        printf("After Thread\n");
                        PROCESSORS++;
                }
        }
}
// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
void *Processor(Job job)
{
        printf("Printing %d from Thread \n",job.number);
        printf("before: %d\n", job.execution_time);
        job.execution_time -= 10;
        printf("after: %d\n", job.execution_time);
        return NULL;
}
void ResourceManager(){

}
void MemoryManager(){

}
/* == Not comfortable woth it!(must be table) === */
/*void MemoryManager(Job job){
   if(job.memory_requirement <= sizeof(memoryManager)){
    memoryManager -= job.memory_requirement;
   }
   }*/
void ResourceManager(){

}
int checkAvaliableResources(Job job){
        return (!(job.resources.resource_A && resourcesManager.resource_A) &&
                !(job.resources.resource_B && resourcesManager.resource_B) &&
                !(job.resources.resource_C && resourcesManager.resource_C) &&
                !(job.resources.resource_D && resourcesManager.resource_D));
}


void reserveResources(Job job){
        resourcesManager.resource_A = job.resources.resource_A;
        resourcesManager.resource_B = job.resources.resource_B;
        resourcesManager.resource_C = job.resources.resource_C;
        resourcesManager.resource_D = job.resources.resource_D;
}
void releaseResources(Job job){
        if(job.resources.resource_A) {
                resourcesManager.resource_A = 0;
        }
        if(job.resources.resource_B) {
                resourcesManager.resource_B = 0;
        }
        if(job.resources.resource_C) {
                resourcesManager.resource_C = 0;
        }
        if(job.resources.resource_D) {
                resourcesManager.resource_D = 0;
        }
}
int Possibility(){
        int possibility;
        possibility = Random(1,10);
        if(possibility == 1) {
                return 1;
        }else{
                return 0;
        }
}
int Random(int min, int max){
        return (min + (rand() % (max - min + 1)));
}
struct tm *getDate(){
        time_t t;
        struct tm *date;
        time(&t);
        date = localtime(&t);
        return date;
}
void writeLog(const char * filename){

}
int main(int argc, char *argv[]){

        /*set srand (seed) once to prevent repeate values in seconds*/
        time_t t;
        srand((unsigned) time(&t));

        /* Intializes jobs queue for SchedulerJobs*/
        JobsQueue = new_queue();

        /*set signal to force quit programm (try without it! and press CTRL+C)*/
        signal(SIGINT,handler);
        /* start run program */
        Generator();

        return 0;
}
