#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "DataStructures2.h"
#include <fcntl.h>

FILE* Processor1Log;
void handler(int sig){
  fclose(Processor1Log);
  exit(0);
}

int main()
{
//         int Processor=open("./log/Processors1.txt",O_TRUNC|O_CREAT|O_WRONLY,0666);
//         close(1);
//         dup(Processor);
          signal(SIGINT, handler);

          Processor1Log = fopen ("./log/Processors1.log", "w+");
          if(!Processor1Log)
            perror("open");
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
                        // ate();
                        fprintf(Processor1Log,"\t\t ->Received job number=: %d \n",
                               j.number);
                        s1.P_1= 1;
////////////////////////////////////////////////////////////////////////////
                        ///////////////Do the prossing//////////////////////
                        for (int i = 0; i < j.execution_time; i++) {
                                usleep(1000);//sleep for fraction of second
                        }
                        sleep(1);
////////////////////////////////////////////////////////////////////////////
                        //Relase the Resources after finsh the prossing//////////////////////
                        msgid = msgget(key, 0666 | IPC_CREAT);
                        msgsnd(msgid, &s1, sizeof(message), 0);

                        j.mtype=11;
                        // printDate();
                        fprintf(Processor1Log,"%s\t\tFinish from job number=%d\n",printDate(), j.number );
                        msgid = msgget(key, 0666 | IPC_CREAT);
                        msgsnd(msgid, &j, sizeof(Job), 0);

                }
        }
        // to destroy the message queue
        msgctl(msgid, IPC_RMID, NULL);
        // close(out);
        return 0;
}
