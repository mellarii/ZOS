#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
  setvbuf(stdout, NULL, _IONBF,0);
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    return 1;
  } else if (pid == 0) {
    printf("CHILD : PID = %d, PPID = %d\n", getpid(), getppid());
    return 0;
  } else {
    printf("PARENT : PID = %d, PPID = %d, childe's PID = %d\n", getpid(), getppid(), pid);
  }
  

  return 0;
}