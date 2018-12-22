#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include "DataStructures2.h"
void exitfunc();
int memory = 2000;
Resources resourcesManager = {0,0,0,0};

int checkAvaliableResources(Resources);
void reserveResources(Job);
void releaseResources(Job);
int main()
{
        ////////////////////////////////////////////////////////////////////////////////
        key_t key;
        // ftok to generate unique key
        key = ftok("prog.c", 65);
        int msgid;
        Job j;
        message s1;
        s1.mtype=5;//change this
        ////////////////////////////////////////////////////////////////////////////////
        while (1) {
                msgid = msgget(key, 0666 | IPC_CREAT);   //put this line every time you wnat to send/recevie a message
                if ((msgrcv(msgid, &j, sizeof(Job),3, IPC_NOWAIT))>1) {
                        // display the message //delete

                        //////////////////////////////////////////////////////////////////
                        if ((memory-j.memory_requirement>0)&&(checkAvaliableResources(resourcesManager))) {
                                memory-=j.memory_requirement;
                                reserveResources(j);

                                printf("reso %d\n",resourcesManager.resource_A );//FOR TEST
                                printf("reso %d\n",resourcesManager.resource_B );//FOR TEST
                                printf("reso %d\n",resourcesManager.resource_C );//FOR TEST
                                printf("reso %d\n",resourcesManager.resource_D );//FOR TEST


                                printf("memory size =%d\n",memory );
                                s1.Resources_Ava=1;
                                msgid = msgget(key, 0666 | IPC_CREAT);
                                msgsnd(msgid, &s1, sizeof(message), 0);
                        }
                        else{
                                s1.Resources_Ava=0;
                                msgid = msgget(key, 0666 | IPC_CREAT);
                                msgsnd(msgid, &s1, sizeof(message), 0);

                        }
                        ////////////////////////////get message from Processors//////////////////////////////////////
                }
                msgid = msgget(key, 0666 | IPC_CREAT);  //put this line every time you wnat to send/recevie a message
                if ((msgrcv(msgid, &j, sizeof(Job),11, IPC_NOWAIT))>1) {
                        printf("\tbefore Resources= %d\n",checkAvaliableResources(resourcesManager) );

                        memory+=j.memory_requirement;
                        printf("\t memory size %d\n",memory);//delete
                        releaseResources(j);
                        printf("\tafter Resources= %d\n",checkAvaliableResources(resourcesManager) );
                        sleep(3);
                }











        }
        // atexit(exitfunc);
        // to destroy the message queue
        // msgctl(msgid, IPC_RMID, NULL);
        return 0;
}


// void exitfunc(){
//         msgctl(msgid, IPC_RMID, NULL);
//
// }


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
