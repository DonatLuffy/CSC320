#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include "DataStructures2.h"

int memory = 2000;
void reserveResources(Job);
void releaseResources(Job);
void handler(int sig);
Resources resourcesManager = {0,0,0,0};
int checkAvaliableResources(Job);
int temp;//we use temp to remove the msgid from the system after we get kill Signal
FILE* MemRes;
int main()
{
        signal(SIGINT,handler);
        MemRes = fopen ("./log/MemRes.log", "w+");
        if(!MemRes)
          perror("open");
////////////////////////////////////////////////////////////////////////////////
        key_t key;
        // ftok to generate unique key
        key = ftok("prog.c", 65);
        int msgid;
        Job j;
        message s1;
        s1.mtype=5;//change this
////////////////////////////////////////////////////////////////////////////////
        fprintf(MemRes,"\t\t--------\n");
        fprintf(MemRes,"\t\tMemory=%d\n",memory );
        fprintf(MemRes,"\t\t--------\n");
        while (1) {
                msgid = msgget(key, 0666 | IPC_CREAT);   //put this line every time you wnat to send/recevie a message
                temp=msgid;
                //cheack if we have a request for resources form the Scheduler
                if ((msgrcv(msgid, &j, sizeof(Job),3, IPC_NOWAIT))>1) {

                        //cheack if we can reserve the resources if not send wait message to the Scheduler
                        if ((memory-j.memory_requirement>0)&&(checkAvaliableResources(j))) {
                                memory-=j.memory_requirement;
                                reserveResources(j);//reserve the resources
                                fprintf(MemRes,"%s\t\t-> Job number=%d Wants: %d megabyte\n",printDate(), j.number, j.memory_requirement );//FOR TEST
                                fprintf(MemRes,"%s\t\tRequested resources A[%d] B[%d] D[%d] C[%d]\n",printDate(), j.resources.resource_A,j.resources.resource_B,j.resources.resource_C,j.resources.resource_D);//FOR TEST

                                s1.Resources_Ava=1;
                                msgid = msgget(key, 0666 | IPC_CREAT);
                                msgsnd(msgid, &s1, sizeof(message), 0);
                        }
                        else{
                                s1.Resources_Ava=0;
                                msgid = msgget(key, 0666 | IPC_CREAT);
                                msgsnd(msgid, &s1, sizeof(message), 0);
                        }
                }//for the first if
////////////////////////////Get messages from Processors//////////////////////////////////////
                msgid = msgget(key, 0666 | IPC_CREAT);  //put this line every time you wnat to send/recevie a message
                if ((msgrcv(msgid, &j, sizeof(Job),11, IPC_NOWAIT))>1) {

                        fprintf(MemRes,"\t\t--------\n");
                        fprintf(MemRes,"\t\tMemory=%d\n",memory );
                        fprintf(MemRes,"\t\t--------\n");

                        memory+=j.memory_requirement;
                        releaseResources(j);
                }
        }
        return 0;
}

int checkAvaliableResources(Job job){
	return (!(job.resources.resource_A && resourcesManager.resource_A) &&
          !(job.resources.resource_B && resourcesManager.resource_B) &&
          !(job.resources.resource_C && resourcesManager.resource_C) &&
          !(job.resources.resource_D && resourcesManager.resource_D));
}

void releaseResources(Job m){
        if(m.resources.resource_A) {
                resourcesManager.resource_A = 0;
        }
        if(m.resources.resource_B) {
                resourcesManager.resource_B = 0;
        }
        if(m.resources.resource_C) {
                resourcesManager.resource_C = 0;
        }
        if(m.resources.resource_D) {
                resourcesManager.resource_D = 0;
        }
}

void reserveResources(Job job){
        resourcesManager.resource_A = job.resources.resource_A;
        resourcesManager.resource_B = job.resources.resource_B;
        resourcesManager.resource_C = job.resources.resource_C;
        resourcesManager.resource_D = job.resources.resource_D;
}

void handler(int sig) {
        printf("Signal number%d \n",sig );
        msgctl(temp, IPC_RMID, NULL);
        fclose(MemRes);
        exit(0);
}
