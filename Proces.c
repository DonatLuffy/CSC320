#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// structure for message queue
struct mesg_buffer {
        long mesg_type;
        char mesg_text[100];
} message;


typedef struct Send {
        long mesg_type;
        int number;
}Send;




int main()
{
        Send s1;
        key_t key;
        int msgid;
        // ftok to generate unique key
        key = ftok("prog.c", 65);

        // msgget creates a message queue
        // and returns identifier
        msgid = msgget(key, 0666 | IPC_CREAT);
        message.mesg_type = 1;

        while (1) {
                printf("Write Data : ");
                gets(message.mesg_text);

                // msgsnd to send message
                msgsnd(msgid, &message, sizeof(message), 0);

                if ((msgrcv(msgid, &s1, sizeof(s1), 3, IPC_NOWAIT))>=1) {
                        printf("Data Received is : %d \n",
                               s1.number);

                }




                // display the message
                printf("Data send is : %s \n", message.mesg_text);
        }





        return 0;
}
