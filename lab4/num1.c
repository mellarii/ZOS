#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <stdatomic.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SHM_NAME "/pract4_shmem"
#define LOCK_FILE "/tmp/pract4_sender.lock"
#define MSG_SIZE 128

typedef struct {
    atomic_uint seq;
    char message[MSG_SIZE];
} shared_data;

int main(void) {
    int lock_fd = open(LOCK_FILE, O_CREAT | O_RDWR, 0666);
    if (lock_fd < 0) {
        perror("open lock");
        return 1;
    }

    if (flock(lock_fd, LOCK_EX | LOCK_NB) < 0) {
        if (errno == EWOULDBLOCK) {
            printf("First programm is almost worked.\n");
        } else {
            perror("flock");
        }
        close(lock_fd);
        return 1;
    }

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shm_fd, sizeof(shared_data)) < 0) {
        perror("ftruncate");
        return 1;
    }

    shared_data *shm = mmap(NULL, sizeof(shared_data),
                            PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    atomic_store_explicit(&shm->seq, 0, memory_order_relaxed);
    shm->message[0] = '\0';

    for (;;) {
        time_t now = time(NULL);
        struct tm tm_now;
        localtime_r(&now, &tm_now);

        char timebuf[64];
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm_now);

        unsigned seq = atomic_load_explicit(&shm->seq, memory_order_relaxed);
        atomic_store_explicit(&shm->seq, seq + 1, memory_order_relaxed);

        snprintf(shm->message, MSG_SIZE, "time=%s pid=%d",
                 timebuf, (int)getpid());

        atomic_store_explicit(&shm->seq, seq + 2, memory_order_release);

        sleep(1);
    }

    return 0;
}