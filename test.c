#include <string.h>
#include <stdio.h>
// #include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
// #include <dirent.h>
#include <time.h>
#include <signal.h>


/* set Handler*/
void handler(){

  exit(0);
}
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

Resources resourcesManager = {0,0,0,0};

int checkAvaliableResources(Job job){
	return (!(job.resources.resource_A && resourcesManager.resource_A) &&
          !(job.resources.resource_B && resourcesManager.resource_B) &&
          !(job.resources.resource_C && resourcesManager.resource_C) &&
          !(job.resources.resource_D && resourcesManager.resource_D));
}
int main(int argc, char *argv[]){

  Job job = {1,NULL,234,634,{0,0,0,0}};
  printf("%lu\n", sizeof(job));
  return 0;
}
