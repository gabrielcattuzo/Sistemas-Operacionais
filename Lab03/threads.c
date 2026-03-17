#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int valor = 0; // Variavel global

void *funcao_thread(void *arg) {
  // Thread secundaria
  valor = 10;
  printf("Thread: alterei o valor para %d\n", valor);
  pthread_exit(NULL);
}

int main() {
  pthread_t t1;

  if (pthread_create(&t1, NULL, funcao_thread, NULL) != 0) {
    printf("Erro ao criar thread.\n");
    return 1;
  }

  // Thread principal
  pthread_join(t1, NULL); // Aguarda a thread terminar
  printf("Main: o valor final eh %d\n", valor);

  return 0;
}
