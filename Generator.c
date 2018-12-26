#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include "DataStructures2.h"
// void printDate();
/*functions prototype*/
void Generator();
int Possibility();
int Random(int min, int max);
struct tm *getDate();
void handler();
/* defined Constant*/
#define MAX_JOBS 10000

FILE *GeneratorLog;

/* set Handler*/
void handler(int sig){
  fclose(GeneratorLog);
  exit(0);
}

void Generator(){
        // int Generator=open("./log/Generator.txt",O_TRUNC|O_CREAT|O_WRONLY,0666);
        // printf("%d\n",Generator );


        GeneratorLog = fopen ("./log/Generator.log", "w+");
        if(!GeneratorLog)
          perror("open");

        int num, fd;
        mkfifo("./pipe", 0666);
        // printf("waiting for readers...\n");
        fd = open("./pipe", O_WRONLY);
        int id = 0;
        while(1) {
                int generates_maximum = Random(0,5);//MAX 5 job per second
                Job job;
                for (int i = 0; i < generates_maximum; i++) {
                        usleep(1000);
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
                                // int t=job.number
                                // int w=write(Generator,"\t\tID=%d\t\tGenerate\n",30);
                                // Generator
                                // printDate();
                                fprintf(GeneratorLog,"%s\t\tID=%d\t\tGenerate\n",printDate(), job.number);

                                if ((num = write(fd, &job, sizeof(job))) == -1)
                                        perror("cannot write in pipe");
                                else
                                        printf("\n");
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
        signal(SIGINT, handler);
        /* start run program */
        Generator();

        return 0;
}



// void printDate(){
//         char buffer[26];
//         int millisec;
//         struct tm* tm_info;
//         struct timeval tv;
//
//         gettimeofday(&tv, NULL);
//
//         millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
//         if (millisec>=1000) { // Allow for rounding up to nearest second
//                 millisec -=1000;
//                 tv.tv_sec++;
//         }
//
//         tm_info = localtime(&tv.tv_sec);
//
//         strftime(buffer, 26, " %H:%M:%S", tm_info);
//         printf("%s.%03d", buffer, millisec);
//
// }
