#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdatomic.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHM_NAME "/pract4_shmem"
#define MSG_SIZE 128

typedef struct {
  atomic_uint seq;
  char message[MSG_SIZE];
} shared_data;

int main(void) {
  shared_data *shm = NULL;

  for (;;) {
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0);
    if (shm_fd >= 0) {
      struct stat st;
      if (fstat(shm_fd, &st) == 0 && st.st_size >= (off_t)sizeof(shared_data)) {
        shm = mmap(NULL, sizeof(shared_data), PROT_READ, MAP_SHARED, shm_fd, 0);
        if (shm != MAP_FAILED) {
          close(shm_fd);
          break;
        }
      }
      close(shm_fd);
    }
    struct timespec req = {0, 200000000};
    nanosleep(&req, NULL);
  }

  for (;;) {
    char local[MSG_SIZE];
    unsigned s1, s2;

    for (;;) {
      s1 = atomic_load_explicit(&shm->seq, memory_order_acquire);
      if (s1 & 1u) {
        continue;
      }

      memcpy(local, shm->message, MSG_SIZE);

      s2 = atomic_load_explicit(&shm->seq, memory_order_acquire);
      if (s1 == s2) {
        break;
      }
    }

    local[MSG_SIZE - 1] = '\0';

    time_t now = time(NULL);
    struct tm tm_now;
    localtime_r(&now, &tm_now);

    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm_now);

    printf("my_time=%s my_pid=%d received=\"%s\"\n", timebuf, (int)getpid(), local);
    fflush(stdout);

    struct timespec req = {0, 500000000};
    nanosleep(&req, NULL);
  }

  return 0;
}