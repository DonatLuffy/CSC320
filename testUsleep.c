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
#include "DataStructures.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

int checkAvaliableResources(Resources);
Queue* JobsQueue;

int main( ) {

        for (int i = 0; i < 5; i++) {
                usleep(30000);
                printf("dque %d\n",i );
        }

        return 0;
}
