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
#include <sys/msg.h>
#include "DataStructures.h"


/* Structs */
typedef struct Resources {// 0 = false, 1 = true
  int resource_A;
  int resource_B;
  int resource_C;
  int resource_D;
}Resources;

typedef struct Job {
  long mtype;
  int number;
  struct tm *creation_date;
  int execution_time;
  int memory_requirement;
  Resources resources;
}Job;

Job resciveMessage(){
  key_t key;
	int msqid;
  Job job;
	key = ftok("/Users/Donat/csc320/project", 'A');
	msqid = msgget(key, 0666 | IPC_CREAT);
  msgrcv(msqid, &job, sizeof(job), -4, 0);
  return job;
}
int main(int argc, char *argv[]){

	/**/


  printf("=====\n");
  printf("%d\n", job.number);
  printf("%d\n", job.execution_time);
  printf("%d\n", job.memory_requirement);

  printf("A=%d\tB=%d\tC=%d\tD=%d\n", job.resources.resource_A,job.resources.resource_B,job.resources.resource_C,job.resources.resource_D);

  printf("=====\n");
  return 0;
}
