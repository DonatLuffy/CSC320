#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int g, s,m,p1,p2;

void handler(){
        kill(g,SIGINT);
        kill(s,SIGINT);
        kill(p1,SIGINT);
        kill(p2,SIGINT);
        kill(m,SIGINT);
        exit(0);
}

int main(int argc, char *argv[]){

        signal(SIGINT, handler);

        if((g = fork())==0) {
                execl("./Generator.o", "Generator", 0);
        }else if((m = fork())==0) {
                execl("./MemRes_Manager.o", "MemRes_Manager", 0);
        }else if((p1 = fork())==0) {
                execl("./Processor1.o", "Processor1", 0);
        }else if((p2 = fork())==0) {
                execl("./Processor2.o", "Processor2", 0);
        }else if((s = fork())==0) {
                execl("./Scheduler.o", "Scheduler", 0);
        }else{// Parent goes here
                waitpid(g,NULL,0);
                waitpid(s,NULL,0);
                waitpid(p1,NULL,0);
                waitpid(p2,NULL,0);
                waitpid(m,NULL,0);
        }
        return 0;
}
