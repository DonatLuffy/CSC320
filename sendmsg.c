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

void sendMessage(){
	key_t key;
	int msqid;
	key = ftok("/Users/Donat/csc320/project", 'A');
	msqid = msgget(key, 0666 | IPC_CREAT);
	Resources resources = {1,0,1,0};
	Job job = {-4,3245,NULL,234,634,resources};
	msgsnd(msqid, &job, sizeof(job), 0);
}
int main(int argc, char *argv[]){

	sendMessage();
  return 0;
}
