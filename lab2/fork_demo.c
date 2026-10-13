#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void bye() {
  printf("[PID=%d] atexit: process is ended\n", getpid());
}

int main() {
  setvbuf(stdout, NULL, _IONBF,0);
  
  atexit(bye);
  
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    return 1;
  } else if (pid == 0) {
    printf("CHILD : PID = %d, PPID = %d\n", getpid(), getppid());
    return 0;
  } else {
    printf("PARENT : PID = %d, PPID = %d, childe PID = %d\n", getpid(), getppid(), pid);
    int status;
    pid_t waited = wait(&status);
    if (waited == -1) {
      perror("wait");
    } else if (WIFEXITED(status)) {
      printf("PARENT : child %d exited normally, code = %d\n", waited, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      printf("PARENT : child %d killed by signal %d\n", waited, WTERMSIG(status));
    }
  }
  
  
  return 0;
}