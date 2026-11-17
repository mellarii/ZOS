#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include <time.h>

#define NUM_READERS 10
#define ARRAY_SIZE 128

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char shared_array[ARRAY_SIZE] = "Initial State";
unsigned long record_counter = 0;

void* writer_thread(void* arg) {
  (void)arg;
  while (1) {
    pthread_mutex_lock(&mutex);
    record_counter++;
    snprintf(shared_array, ARRAY_SIZE, "Record %lu", record_counter);
    pthread_mutex_unlock(&mutex);
    struct timespec ts = {1, 0};
    nanosleep(&ts, NULL);
  }
  return NULL;
}

void* reader_thread(void* arg) {
  int id = *(int*)arg;
  free(arg);

  while (1) {
    char local_buf[ARRAY_SIZE];

    pthread_mutex_lock(&mutex);
    strncpy(local_buf, shared_array, ARRAY_SIZE);
    pthread_mutex_unlock(&mutex);

    long tid = syscall(SYS_gettid);
    printf("Reader %2d | TID: %ld | Array state: \"%s\"\n", id, tid, local_buf);
    fflush(stdout);

    struct timespec ts = {0, 500000000};
    nanosleep(&ts, NULL);
  }
  return NULL;
}

int main(void) {
  pthread_t writer;
  pthread_t readers[NUM_READERS];

  if (pthread_create(&writer, NULL, writer_thread, NULL) != 0) {
    perror("pthread_create writer");
    return 1;
  }

  for (int i = 0; i < NUM_READERS; i++) {
    int *id = malloc(sizeof(int));
    *id = i + 1;
    if (pthread_create(&readers[i], NULL, reader_thread, id) != 0) {
      perror("pthread_create reader");
      return 1;
    }
  }

  pthread_join(writer, NULL);
  for (int i = 0; i < NUM_READERS; i++) {
    pthread_join(readers[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
  return 0;
}