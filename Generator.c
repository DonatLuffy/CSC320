#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
int Possibility();
int Random(int min, int max);
struct tm *getDate();
void handler();
/* defined Constant*/
#define MAX_JOBS 10000

/* set Handler*/
void handler(){
        exit(0);
}

void Generator(){
        int num, fd;
        mkfifo("./pipe", 0777);
        printf("waiting for readers...\n");
        fd = open("./pipe", O_WRONLY);
        int id = 0;
        while(1) {
                int generates_maximum = Random(0,5);//MAX 5 job per second
                Job job;
                for (int i = 0; i < generates_maximum; i++) {
                        if(id < MAX_JOBS) {
                                job.mtype=3; //3 will go to the resources Manager
                                job.number = id++;
                                job.creation_date = getDate();
                                job.execution_time = Random(10,1000);
                                job.memory_requirement = Random(1,100);
                                job.resources.resource_A = Possibility();
                                job.resources.resource_B = Possibility();
                                job.resources.resource_C = Possibility();
                                job.resources.resource_D = Possibility();

                                //TODO write log here
                                printf("=====\n");
                                printf("%ld\n", job.mtype);
                                printf("%d\n", job.number);
                                printf("%d\n", job.execution_time);
                                printf("%d\n", job.memory_requirement);
                                printf("A=%d\tB=%d\tC=%d\tD=%d\n", job.resources.resource_A,job.resources.resource_B,job.resources.resource_C,job.resources.resource_D);
                                printf("=====\n");
                                if ((num = write(fd, &job, sizeof(job))) == -1)
                                        perror("cannot write in pipe");
                                else
                                        printf("producer: wrote %d bytes\n", num);
                        }
                        else
                                exit(0); //exit if id > MAX_JOBS
                }//for the for
                sleep(1);
        }//for the while
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

int main(int argc, char *argv[]){
        /*set srand (seed) once to prevent repeate values in seconds*/
        time_t t;
        srand((unsigned) time(&t));

        /* start run program */
        Generator();

        return 0;
}
