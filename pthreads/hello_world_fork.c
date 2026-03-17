#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *hello_world(void *arg) {
  int num_thread  = *(int *)arg;
  printf("Hello World da thread %d!\n", num_thread);

  pthread_exit(0);
}

int main(int argc, char *argv[]) {
  printf("Estou digitando da thread principal!\n");

  int NUM_THREADS = atoi(argv[1]);

  pthread_t *thread_ids = malloc(sizeof(pthread_t) * NUM_THREADS);

  int * ids = malloc(sizeof(int) * NUM_THREADS); 
  for (int i = 0 ; i < NUM_THREADS ; ++i)  {
    ids[i] = i;
    pthread_create(&thread_ids[i], NULL, hello_world, &ids[i]);
  }

  for (int i = 0 ; i < NUM_THREADS ; ++i) 
    pthread_join(thread_ids[i], NULL);

  free(thread_ids);
  free(ids);
  return 0;
}
