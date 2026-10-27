#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define FIFO_PATH "/tmp/lab3_fifo"

void get_time(char *buffer, size_t size) {
    time_t rawtime = time(NULL);
    struct tm *timeinfo = localtime(&rawtime);
    strftime(buffer, size, "%H:%M:%S", timeinfo);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <writer|reader>\n", argv[0]);
        return 1;
    }

    char time_buf[64];
    char message[256];

    if (strcmp(argv[1], "writer") == 0) {
        mkfifo(FIFO_PATH, 0666);
        int fd = open(FIFO_PATH, O_WRONLY);
        if (fd == -1) {
            perror("open writer");
            return 1;
        }

        get_time(time_buf, sizeof(time_buf));
        snprintf(message, sizeof(message), "Writer PID: %d, Sent at: %s", getpid(), time_buf);

        sleep(10);

        write(fd, message, strlen(message) + 1);
        close(fd);
    } else if (strcmp(argv[1], "reader") == 0) {
        int fd = open(FIFO_PATH, O_RDONLY);
        if (fd == -1) {
            perror("open reader");
            return 1;
        }

        read(fd, message, sizeof(message));
        get_time(time_buf, sizeof(time_buf));
        
        printf("Reader PID: %d, Time: %s\n", getpid(), time_buf);
        printf("Reader received: %s\n", message);
        
        close(fd);
    } else {
        printf("Invalid argument. Use 'writer' or 'reader'.\n");
    }

    return 0;
}