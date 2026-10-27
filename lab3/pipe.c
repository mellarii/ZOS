#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

void get_time(char *buffer, size_t size) {
    time_t rawtime = time(NULL);
    struct tm *timeinfo = localtime(&rawtime);
    strftime(buffer, size, "%H:%M:%S", timeinfo);
}

int main() {
    int fd[2];
    pid_t pid;
    char time_buf[64];
    char message[256];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        close(fd[1]);
        read(fd[0], message, sizeof(message));
        close(fd[0]);

        get_time(time_buf, sizeof(time_buf));
        printf("Child PID: %d, Time: %s\n", getpid(), time_buf);
        printf("Child received: %s\n", message);
    } else {
        close(fd[0]);
        get_time(time_buf, sizeof(time_buf));
        snprintf(message, sizeof(message), "Parent PID: %d, Sent at: %s", getpid(), time_buf);

        sleep(5); 

        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
        wait(NULL);
    }

    return 0;
}