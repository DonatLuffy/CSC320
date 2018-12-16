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

/* Structs */
typedef struct Resources {// 0 = false, 1 = true
  int resource_A;
  int resource_B;
  int resource_C;
  int resource_D;
}Resources;

typedef struct Job {
  int number;
  struct tm *creation_date;
  int execution_time;
  int memory_requirement;
  Resources resources;
}Job;


/*functions prototype*/
void Generator();
void SchedulerJobs();
int Possibility();
int Random();
struct tm *getDate();
void handler();
void* Processor();

/* defined Constant*/
#define MAX_JOBS 5
char memory[2000];
int PROCESSORS = 2;/*number of Processors avaliable*/

/* declare Queue*/
Queue* JobsQueue;

/* set Handler*/
void handler(){
  Job* job;
  job = (Job*) dequeue(JobsQueue);
  if(job)
    printf("%d\n", job->number);
  exit(0);
}

void Generator(){
  while(1){
    int i, id;
    int generates_maximum = Random(0,5);
    Job job;
    for (i = 0; i < generates_maximum; i++) {
      if(id < MAX_JOBS){
        job.number = id++;
      }
      job.creation_date = getDate();
      job.execution_time = Random(10,1000);
      job.memory_requirement = Random(1,100);
      job.resources.resource_A = Possibility();
      job.resources.resource_B = Possibility();
      job.resources.resource_C = Possibility();
      job.resources.resource_D = Possibility();
      SchedulerJobs(job);
    }
    sleep(1);
  }
}
void SchedulerJobs(Job job){
	printf("%d\n", job.number);
  enqueue(JobsQueue, &job);
	if(PROCESSORS != 0){
		PROCESSORS--;
	  pthread_t thread_id;
	  printf("Before Thread\n");
	  pthread_create(&thread_id, NULL, Processor, ((Job*)dequeue(JobsQueue)));
	  pthread_join(thread_id, NULL);
	  printf("After Thread\n");
		PROCESSORS++;
 	}
}
// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
void *Processor(Job job)
{
	sleep(2);
	printf("Printing %d from Thread \n",job.number);
	return NULL;
}

void MemoryManager(){

}
void ResourceManager(Resources resources){

}

int Possibility(){
  int possibility;
  possibility = Random(1,10);
  if(possibility == 1){
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
int main(){
	/*set srand (seed) once to prevent repeate values in seconds*/
	time_t t;
	srand((unsigned) time(&t));

	/* Intializes jobs queue for SchedulerJobs*/
	JobsQueue = new_queue();

	/*set sigmal to force quit programm (try without it! and press CTRL+C)*/
	signal(SIGINT,handler);
	printf("%lu\n", sizeof(memory));
	/* start run program */
	Generator();

	exit(0);
}
