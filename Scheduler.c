#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
// #include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include "DataStructures2.h"
#include <sys/ipc.h>
#include <sys/msg.h>
void Processor(Job job);
/*functions prototype*/
void SchedulerJobs();
// int Possibility();
// int Random(int min, int max);
struct tm *getDate();
void handler(int );
void Processor();
int checkAvaliableResources(Resources);
void releaseResources(message);
void reserveResources(Job);

int P_1 = 1;//the Processors number | 0=not avaliable
int P_2 = 1;//the Processors number | 0=not avaliable
int Resources_Ava=0; //flag for cheack the Resources avaliabality

/*Intializes resources*/
// Resources resourcesManager = {0,0,0,0};

/* declare Queue*/
// Queue* JobsQueue;

/* set Handler*/
void handler(int sig){
        printf("\t signal number%d\n",sig );
        exit(0);
}
Queue* JobsQueue;

void SchedulerJobs(){
        int i=0;//for test
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
                        printf("\t\t add to que %d\n",job.number );
                        enqueue(JobsQueue, job);//add the job to the Queue
                        i++;
//////////////////Reserve-The Resourcess By ask the resourcesManager//////////////////////////////////
                        Job j=dequeue(JobsQueue);//dequeue a job from JobsQueue
                        // Resources_Ava=0;//fix this
                        do {
                                printf("\t wait for Resources= %d\n",Resources_Ava );
                                msgid = msgget(key, 0666 | IPC_CREAT);
                                msgsnd(msgid, &j, sizeof(Job), 0);//message type =3 by default
                                msgid = msgget(key, 0666 | IPC_CREAT);
                                if ((msgrcv(msgid, &m, sizeof(message),5, 0))<0) {
                                        perror("cannot read form resourcesManager");
                                }
                                Resources_Ava=m.Resources_Ava;
                                printf("\t insie while Resources= %d\n",Resources_Ava );//delete
                        } while(!Resources_Ava);
/////////////////////////////////////////////////////////////////////////////////////////////////
                        //check if there a new mail from Processor number 1
checkMailBox:           msgid = msgget(key, 0666 | IPC_CREAT); //put this line when send/recevie a message
                        if ((msgrcv(msgid, &m, sizeof(message),9, IPC_NOWAIT))>0) {
                                P_1=m.P_1;
                        }
/////////////////////////////////////////////////////////////////////////////////////////////////
                        //check if there a new mail from Processor number 2

                        msgid = msgget(key, 0666 | IPC_CREAT); //put this line when send/recevie a message
                        if ((msgrcv(msgid, &m, sizeof(message),8, IPC_NOWAIT))>0) {
                                P_2=m.P_2;
                        }
/////////////////////////////////////////////////////////////////////////////////////////////////
                        if (P_1==1) {//if processor number 1 avaliable send the job to it
                                P_1=0;
                                j.mtype=1;
                                msgid = msgget(key, 0666 | IPC_CREAT); //put this line every time you wnat to send/recevie a message
                                msgsnd(msgid, &j, sizeof(Job), 0);
                                printf("\t send to processor 1\n" );

                        }
                        else if (P_2==1) {//else if processor number 2 avaliable send the job to it
                                P_2=0;
                                j.mtype=2;
                                msgid = msgget(key, 0666 | IPC_CREAT); //put this line every time you wnat to send/recevie a message
                                msgsnd(msgid, &j, sizeof(Job), 0);
                                printf("\t send to processor 2\n" );

                        }
                        else{  //else wait for any processor to be avaliable(go to check mail box)
                                printf("\t goto\n");
                                usleep(8000);
                                goto checkMailBox;
                        }
/////////////////////////////////////////////////////////////////////////////////////////////////











                }
                sleep(1);
        }//while(1)
        close(fd);      //close the FIFO
}//void SchedulerJobs

// int Possibility(){
//         int possibility;
//         possibility = Random(1,10);
//         if(possibility == 1) {
//                 return 1;
//         }else{
//                 return 0;
//         }
// }

// int Random(int min, int max){
//         return (min + (rand() % (max - min + 1)));
// }
// void Processor(Job job){
//         Job j=job;
//         printf("@@@@@@@@@@@@@@@@@@@@@@@@\n");
//         printf("=====\n");
//         printf("%d\n", j.number);
//         printf("%d\n", j.execution_time);
//         printf("%d\n", j.memory_requirement);
//         printf("A=%d\tB=%d\tC=%d\tD=%d\n", j.resources.resource_A,j.resources.resource_B,j.resources.resource_C,j.resources.resource_D);
//         exit(0);
//
// }
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
        /* Intializes jobs queue for SchedulerJobs*/
        JobsQueue = new_queue();
        /*set sigmal to force quit programm (try without it! and press CTRL+C)*/
        signal(SIGINT,handler);
        /* start run program */
        SchedulerJobs();
        return 0;
}


int checkAvaliableResources(Resources r){

        // printf("\t\t final %d\n",(r.resource_A & r.resource_B)&(r.resource_C &   r.resource_D) );
        if (r.resource_A==1)
                return (0);
        if (r.resource_B==1)
                return (0);
        if (r.resource_C==1)
                return (0);
        if (r.resource_D==1)
                return (0);
        return (1);
}



// void releaseResources(message m){
//         if(m.resources.resource_A) {
//                 resourcesManager.resource_A = 0;
//         }
//         if(m.resources.resource_B) {
//                 resourcesManager.resource_B = 0;
//         }
//         if(m.resources.resource_C) {
//                 resourcesManager.resource_C = 0;
//         }
//         if(m.resources.resource_D) {
//                 resourcesManager.resource_D = 0;
//         }
// }



// void reserveResources(Job job){
//         resourcesManager.resource_A = job.resources.resource_A;
//         resourcesManager.resource_B = job.resources.resource_B;
//         resourcesManager.resource_C = job.resources.resource_C;
//         resourcesManager.resource_D = job.resources.resource_D;
// }







void send_tO_Pross(Job j,int msgid) {


        // if (P_1==1) {     //Checks if the processor 1 is avaliable or not
        //         P_1=0;    //lock the processor because it used

        // j=*(Job*)dequeue(JobsQueue);  //for DataStructures.h test
        // j=dequeue(JobsQueue);   //new style
        printf("\t send to prosse: %d \n",j.number );

        // reserveResources(j);
        // msgid = msgget(key, 0666 | IPC_CREAT);   //put this line every time you wnat to send/recevie a message
        if ((msgsnd(msgid,&j,sizeof(Job),0) )<1) {
                perror("prosser didnt get the mass ");
        }
        // sleep(3);
        // continue;
        // }
        //TODO add processor number 2
        // else if (P_2==1) { //Checks if the processor 1 is avaliable or not
        //         P_2=0;
        // }
        // else
        //         continue;
}
/////////////////////////////////////////////////////////////////////////////////////////////////}
