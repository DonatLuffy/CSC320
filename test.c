#include <string.h>
#include <stdio.h>
// #include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
// #include <dirent.h>
#include <time.h>

/*
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
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf ( "Current local time and date: %s", asctime (timeinfo) );
    sleep(1);
  }
}
void writeLog(const char * filename){
  char buf1[12] = "hello world";
  int fd; /* file descriptor
  int nbytes; /* number of bytes read
  if((fd = open(filename, O_WRONLY | O_APPEND)) < 0){
    perror("open");
    exit(1);
  }
  write(fd, buf1, strlen(buf1));
  int retval; /* return value
  if ((retval = close(fd)) < 0) {
  perror("close");
  exit(1);
  }
}*/


int main(int argc, char *argv[]){

  char *str = (char *) malloc( 3 * sizeof(char));
  *str = 'H'; *(str+1) = 'i'; *(str+2) = '\0';
  str = (char *) realloc( str , 6 * sizeof(char) );
  *(str+1) = 'e'; *(str+2) = 'l'; *(str+3) = 'l'; *(str+4) = 'o'; *(str+5) = '\0';
  printf("%s\n", str);
  return 0;
}
