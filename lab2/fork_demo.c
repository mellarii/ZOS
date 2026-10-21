#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void on_sigint(int signo) {
  printf("[PID=%d] signal(): %s (%d)\n", getpid(), strsignal(signo), signo);
}

void on_sigterm(int signo) {
  printf("[PID=%d] sigaction(): %s (%d)\n", getpid(), strsignal(signo), signo);
}

void bye() {
  printf("[PID=%d] atexit: process is ended\n", getpid());
}

int main() {
  setvbuf(stdout, NULL, _IONBF,0);
  
  signal (SIGINT, on_sigint);
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = on_sigterm;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction(SIGTERM, &sa, NULL);

  atexit(bye);
  
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    return 1;
  } else if (pid == 0) {
    printf("CHILD : PID = %d, PPID = %d\n", getpid(), getppid());
    pause();
    return 0;
  } else {
    printf("PARENT : PID = %d, PPID = %d, child's PID = %d\n", getpid(), getppid(), pid);
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
