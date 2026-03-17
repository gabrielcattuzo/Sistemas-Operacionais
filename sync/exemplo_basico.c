#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LIMITE 100000

long soma = 0;
pthread_mutex_t mutex;

// pthread_mutex_lock()
// pthread_mutex_unlock()

void * somar_variavel(void *args) {
  pthread_mutex_lock(&mutex);
  for (int i = 0 ; i < LIMITE ; ++i) {
    soma++;
  }
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
  int num_threads = atol(argv[1]);

  pthread_t * thread_id = malloc(sizeof(pthread_t) * num_threads);
  pthread_mutex_init(&mutex, NULL);

  // criamos as threads
  for (int i = 0 ; i < num_threads ; ++i)
    pthread_create(&thread_id[i], NULL, somar_variavel, NULL);

  // esperamos elas computarem
  for (int i =0 ; i < num_threads ; ++i)
    pthread_join(thread_id[i], NULL);

  printf("soma => %ld\n", soma);
  pthread_mutex_destroy(&mutex);
  // limpa a memoria
  free(thread_id);
  return 0;
}
