#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
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
  float execution_time;
  float memory_requirement;
  Resources resources;
}Job;


/*functions prototype*/
void Generator();
void _Scheduler(Job );
int _Possibility();
int Random(int , int );
struct tm *getDate();
void handler();

/* defined Constant*/
#define MAX_JOBS 1000

/* declare Queue*/
Queue* queue;

/* set Handler*/
void handler(){
  Job* job = (Job*) dequeue(queue);
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
      job.resources.resource_A = _Possibility();
      job.resources.resource_B = _Possibility();
      job.resources.resource_C = _Possibility();
      job.resources.resource_D = _Possibility();
      _Scheduler(job);
    }
    sleep(1);
  }
}
void _Scheduler(Job job){
  printf("_Scheduler!\n");
  enqueue(queue, &job);
  printf("%d\n", ((Job*)dequeue(queue))->number);
}
void Processors(){
  
}
int _Possibility(){
  int possibility;
  possibility = Random(1,10);
  if(possibility == 1){
    return 1;
  }else{
    return 0;
  }
}
int Random(int min, int max){
  time_t t;
  srand((unsigned) time(&t));
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
  queue = new_queue();
  signal(SIGINT,handler);
  Generator();
  return 0;
}
