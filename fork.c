#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
int main(void)
{
  pid_t parentpid;
  pid_t childpid;
  if ((childpid = fork()) == -1) {
    perror("can’t create a new process");
    exit(1);
  }
  else if (childpid == 0) {/* child process executes */
    printf("child: childpid = %d, parentpid = %d \n", getpid(), getppid());
    execl("/Users/Donat/csc320/project/prog.o","prog",NULL);
    exit(0);
  }
  else { /*parent process executes */
    wait(NULL);
    printf("parent: childpid = %d, parentpid = %d \n", childpid, getpid());
    exit(0);
  }
}
