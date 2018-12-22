#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "DataStructures2.h"
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
                        // sleep(3);
                        // display the message //delete
                        printf("\t Received : %d \n",
                               j.number);
                        s1.P_1= 1;
////////////////////////////////////////////////////////////////////////////
                        ///////////////Do the prossing//////////////////////
                        for (int i = 0; i < j.execution_time; i++) {
                                usleep(19000);//sleep for fraction of second
                        }
                        sleep(1);
////////////////////////////////////////////////////////////////////////////
                        //Relase the Resources after finsh the prossing//////////////////////
                        msgid = msgget(key, 0666 | IPC_CREAT);
                        msgsnd(msgid, &s1, sizeof(message), 0);

                        j.mtype=11;
                        printf("\tfinsh from job number=%d\n",j.number );
                        msgid = msgget(key, 0666 | IPC_CREAT);
                        msgsnd(msgid, &j, sizeof(Job), 0);

                }
        }
        // to destroy the message queue
        // msgctl(msgid, IPC_RMID, NULL);
        return 0;
}
