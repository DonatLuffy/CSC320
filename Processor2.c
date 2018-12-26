#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "DataStructures2.h"

FILE* Processor2Log;
void handler(int sig){
  fclose(Processor2Log);
  exit(0);
}

int main()
{
      signal(SIGINT, handler);

      Processor2Log = fopen ("./log/Processor2.log", "w+");
      if(!Processor2Log)
        perror("open");
////////////////////////////////////////////////////////////////////////////////
        key_t key;
        int msgid;
        // ftok to generate unique key
        key = ftok("prog.c", 65);
        Job j;
        message s1;
        s1.mtype=8;
////////////////////////////////////////////////////////////////////////////////
        while (1) {
                msgid = msgget(key, 0666 | IPC_CREAT); //put this line every time you wnat to send/recevie a message
                if ((msgrcv(msgid, &j, sizeof(Job),2, 0))>0) {
                        // sleep(3);
                        // display the message //delete
                        fprintf(Processor2Log,"%s\t\t ->Received job number=: %d \n",printDate(),
                               j.number);
                        s1.P_2= 1;
////////////////////////////////////////////////////////////////////////////
                        ///////////////Do the prossing//////////////////////
                        for (int i = 0; i < j.execution_time; i++) {
                                usleep(11000);//sleep for fraction of second
                        }
////////////////////////////////////////////////////////////////////////////
                        //Relase the Resources after finsh the prossing//////////////////////
                        msgid = msgget(key, 0666 | IPC_CREAT);
                        msgsnd(msgid, &s1, sizeof(message), 0);

                        j.mtype=11;
                        fprintf(Processor2Log,"%s\t\tFinsh from job number=%d\n",printDate() ,j.number );
                        msgid = msgget(key, 0666 | IPC_CREAT);
                        msgsnd(msgid, &j, sizeof(Job), 0);

                }
        }
        // to destroy the message queue
        // msgctl(msgid, IPC_RMID, NULL);
        return 0;
}
