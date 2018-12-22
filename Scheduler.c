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
#include <sys/ipc.h>
#include <sys/msg.h>
#include "DataStructures2.h"
// void Processor(Job job);
void SchedulerJobs();
void handler(int );
// void Processor();
// void releaseResources(message);
// void reserveResources(Job);
// int checkAvaliableResources(Resources);
struct tm *getDate();

/* set Handler*/
void handler(int);
int P_1 = 1;//the Processors number | 0=not avaliable
int P_2 = 1;//the Processors number | 0=not avaliable
int Resources_Ava=0; //flag for cheack the Resources avaliabality
Queue* JobsQueue;

void SchedulerJobs(){
        // int i=0;//for test
        JobsQueue=new_queue();
        Job j;
        Job job;
        message m;//message form the Processors
        int num, fd;
        mkfifo ("./pipe", 0777);
        fd = open("./pipe", O_RDONLY);
        // printf("got a writer\n");
////////////////////////////////////////////////////////////////////////////////
        key_t key;
        int msgid;
// ftok to generate unique key
        key = ftok("prog.c", 65);
// msgget creates a message queue
// and returns identifier
        msgid = msgget(key, 0666 | IPC_CREAT);
////////////////////////////////////////////////////////////////////////////////
        while(1) {
                Job job;
                if ((num = read(fd, &job, sizeof(job))) == -1) {
                        perror("cannot read");
                        exit(0);
                }else if(num == 0 || num ==1) {
                        printf("end of pipe %d\n",num);
                        exit(0);
                }else {
                        printf("\t\t ->Add job: %d to the queue\n",job.number );
                        enqueue(JobsQueue, job);//add the job to the Queue
                        // i++;
//////////////////Reserve-The Resourcess By ask the resourcesManager//////////////////////////////////
                        Job j=dequeue(JobsQueue);//dequeue a job from JobsQueue
                        // Resources_Ava=0;//fix this
                        printf("\t Waiting for Resources\n");
                        do {
                                msgid = msgget(key, 0666 | IPC_CREAT);
                                msgsnd(msgid, &j, sizeof(Job), 0);//message type =3 by default
                                msgid = msgget(key, 0666 | IPC_CREAT);
                                if ((msgrcv(msgid, &m, sizeof(message),5, 0))<0) {
                                        perror("Cannot read from resourcesManager");
                                }
                                Resources_Ava=m.Resources_Ava;
                        } while(!Resources_Ava);
/////////////////////////////////////////////////////////////////////////////////////////////////
                        //check if there a new mail from Processor number 1
checkMailBox:           msgid = msgget(key, 0666 | IPC_CREAT); //put this line when send/recevie a message
                        if ((msgrcv(msgid, &m, sizeof(message),9, IPC_NOWAIT))>0) {
                                P_1=m.P_1;
                        }
//################################################################################################
                        //check if there a new mail from Processor number 2

                        msgid = msgget(key, 0666 | IPC_CREAT); //put this line when send/recevie a message
                        if ((msgrcv(msgid, &m, sizeof(message),8, IPC_NOWAIT))>0) {
                                P_2=m.P_2;
                        }
/////////////////////////Sendig jobs to processor////////////////////////////////////////////////////
                        if (P_1==1) {//if processor number 1 avaliable send the job to it
                                P_1=0;
                                j.mtype=1;
                                msgid = msgget(key, 0666 | IPC_CREAT); //put this line every time you wnat to send/recevie a message
                                msgsnd(msgid, &j, sizeof(Job), 0);
                                printf("\t Send job to processor: 1\n" );

                        }
                        else if (P_2==1) {//else if processor number 2 avaliable send the job to it
                                P_2=0;
                                j.mtype=2;
                                msgid = msgget(key, 0666 | IPC_CREAT); //put this line every time you wnat to send/recevie a message
                                msgsnd(msgid, &j, sizeof(Job), 0);
                                printf("\t Send job to processor: 2\n" );
                        }
                        else{  //else wait for any processor to be avaliable(go to check mail box)
                                usleep(9000);
                                goto checkMailBox;
                        }
/////////////////////////////////////////////////////////////////////////////////////////////////
                }
                sleep(1);
        }//while(1)
        close(fd);      //close the FIFO
}//void SchedulerJobs

int main(int argc, char *argv[]){
        /*set srand (seed) once to prevent repeate values in seconds*/
        time_t t;
        srand((unsigned) time(&t));
        /* Intializes jobs queue for SchedulerJobs*/
        JobsQueue = new_queue();
        /*set sigmal to force quit programm (try without it! and press CTRL+C)*/
        signal(SIGINT,handler);
        /* start run program */
        SchedulerJobs();
        return 0;
}

struct tm *getDate(){
        time_t t;
        struct tm *date;
        time(&t);
        date = localtime(&t);
        return date;
}

void handler(int sig){
        printf("\t Signal number%d\n",sig);//Print the signal value
        exit(0);
}
