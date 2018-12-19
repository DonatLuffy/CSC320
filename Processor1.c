#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "DataStructures.h"
int main()
{
////////////////////////////////////////////////////////////////////////////////
        key_t key;
        int msgid;
        // ftok to generate unique key
        key = ftok("prog.c", 65);
        Job j;
        message s1;
        s1.mtype=9;
////////////////////////////////////////////////////////////////////////////////
        while (1) {
                msgid = msgget(key, 0666 | IPC_CREAT); //put this line every time you wnat to send/recevie a message
                if ((msgrcv(msgid, &j, sizeof(Job),1, 0))>0) {
                        sleep(3);
                        // display the message
                        printf("\t Received : %d \n",
                               j.number);

                        //send data to the Scheduler
                        s1.memory=j.memory_requirement;
                        s1.P_1=1;
                        s1.resources=j.resources;

////////////////////////////////////////////////////////////////////////////
                        ///////////////Do the prossing//////////////////////

                        for (int i = 0; i < j.execution_time; i++) {
                                sleep(0);
                        }
////////////////////////////////////////////////////////////////////////////

                        // printf("memory relase =: %d \n",
                        //        j.memory_requirement);
                        // printf("resource_A =: %d \n",
                        //        s1.resources.resource_A);
                        // printf("resource_A =: %d \n",
                        //        s1.resources.resource_B);
                        // printf("resource_A =: %d \n",
                        //        s1.resources.resource_C);
                        // printf("resource_A =: %d \n",
                        //        s1.resources.resource_D);
                        ///////////////Relase the Resources//////////////////////
                        msgid = msgget(key, 0666 | IPC_CREAT);
                        msgsnd(msgid, &s1, sizeof(message), 0);
                }
        }
        // to destroy the message queue
        // msgctl(msgid, IPC_RMID, NULL);
        return 0;
}
