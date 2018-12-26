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
void SchedulerJobs();
void handler(int );
struct tm *getDate();

/* set Handler*/
void handler(int);
int P_1 = 1;//the Processors number | 0=not avaliable
int P_2 = 1;//the Processors number | 0=not avaliable
int Resources_Ava=0; //flag for cheack the Resources avaliabality
Queue* JobsQueue;
FILE* SchedulerLog;

void SchedulerJobs(){
        SchedulerLog = fopen ("./log/Scheduler.log", "w+");
        if(!SchedulerLog)
          perror("open");
////////////////////////////////////////////////////////////////////////////////
        int num, fd;
        mkfifo ("./pipe", 0666);
        //TODO remove the pipe after finshing
        fd = open("./pipe", O_RDONLY);
        JobsQueue=new_queue();
        Job job;
        message m;//message form the Processors
        key_t key;
        int msgid;
// ftok to generate unique key
        key = ftok("prog.c", 65);
// msgget creates a message queue
        msgid = msgget(key, 0666 | IPC_CREAT);
////////////////////////////////////////////////////////////////////////////////
        while(1) {    //read jobs form the FIFO and add them to JobsQueue
                if ((num = read(fd, &job, sizeof(job))) == -1) {
                        perror("Cannot read from the FIFO");
                        exit(0);
                }else if(num == 0 || num ==1) {
                        fprintf(SchedulerLog,"End of pipe %d\n",num);
                        exit(0);
                }else {
                        // printDate();
                        fprintf(SchedulerLog,"%s\t\t->Add job: %d to the queue\n",printDate() ,job.number );
                        enqueue(JobsQueue, job);//add the job to the Queue
//////////////////Reserve-The Resourcess By ask the resourcesManager//////////////////////////////////
                        job=dequeue(JobsQueue); //dequeue a job from JobsQueue
                        // printDate();
                        // fprintf(SchedulerLog,"%s\t\tWaiting for Resources\n",printDate());
                        do {//send a mail to MemRes_Manager ask him for resourcess
                                msgid = msgget(key, 0666 | IPC_CREAT);
                                msgsnd(msgid, &job, sizeof(Job), 0);//send the request

                                msgid = msgget(key, 0666 | IPC_CREAT);
                                if ((msgrcv(msgid, &m, sizeof(message),5, 0))<0) {//reserve the answer
                                        perror("Cannot read from resourcesManager");
                                }
                                Resources_Ava=m.Resources_Ava;
                        } while(!Resources_Ava); //if we did't have the resourcess wait for it
/////////////////////////////////////////////////////////////////////////////////////////////////
                        //Check if there a new mail from Processor number 1
checkMailBox:           msgid = msgget(key, 0666 | IPC_CREAT); //put this line when send/recevie a message
                        if ((msgrcv(msgid, &m, sizeof(message),9, IPC_NOWAIT))>0) {
                                P_1=m.P_1;
                        }
//################################################################################################
                        //Check if there a new mail from Processor number 2
                        msgid = msgget(key, 0666 | IPC_CREAT); //put this line when send/recevie a message
                        if ((msgrcv(msgid, &m, sizeof(message),8, IPC_NOWAIT))>0) {
                                P_2=m.P_2;
                        }
/////////////////////////Sendig jobs to processor////////////////////////////////////////////////////
                        if (P_1==1) {//if processor number 1 avaliable send the job to it
                                P_1=0;
                                job.mtype=1;
                                msgid = msgget(key, 0666 | IPC_CREAT); //put this line every time you wnat to send/recevie a message
                                msgsnd(msgid, &job, sizeof(Job), 0);
                                // printDate();
                                fprintf(SchedulerLog,"\t\t->Dispatched job ID= %d to Processor1\n",job.number );
                                // fprintf(SchedulerLog,"\t Send job to processor: 1\n" );

                        }
                        else if (P_2==1) {//else if processor number 2 avaliable send the job to it
                                P_2=0;
                                job.mtype=2;
                                msgid = msgget(key, 0666 | IPC_CREAT); //put this line every time you wnat to send/recevie a message
                                msgsnd(msgid, &job, sizeof(Job), 0);
                                // printDate();
                                fprintf(SchedulerLog,"\t\t->Dispatched job ID= %d to Processor2\n",job.number );
                                // fprintf(SchedulerLog,"\t Send job to processor: 2\n" );
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
  fclose(SchedulerLog);
  exit(0);
}
