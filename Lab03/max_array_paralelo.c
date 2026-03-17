#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  int *vetor;
  long limite_inferior;
  long limite_superior;
  int maximo_local;
} ThreadArgs;

double calcula_tempo(struct timespec start, struct timespec end) {
  return (end.tv_sec - start.tv_sec) * 1000.0 +
         (double)(end.tv_nsec - start.tv_nsec) / 1000000.0;
}

void *encontra_maximo_local(void *arg) {

  ThreadArgs *args = (ThreadArgs *)arg;

  int *vetor = args->vetor;
  long inicio = args->limite_inferior;
  long fim = args->limite_superior;

  int max = vetor[inicio];

  for (long i = inicio + 1; i < fim; i++) {
    if (vetor[i] > max) {
      max = vetor[i];
    }
  }

  args->maximo_local = max;

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  if (argc < 3) {
    fprintf(stderr, "Uso: %s <tamanho_vetor> <num_threads>\n", argv[0]);
    return 1;
  }

  long tamanho = atol(argv[1]);
  int num_threads = atoi(argv[2]);

  int *vetor = malloc(tamanho * sizeof(int));

  srand(time(NULL));
  for (long i = 0; i < tamanho; i++) {
    vetor[i] = rand() % 1000000;
  }

  vetor[tamanho / 2] = 2000000;

  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  ThreadArgs *args = malloc(num_threads * sizeof(ThreadArgs));

  long fatia = tamanho / num_threads;

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for (int i = 0; i < num_threads; i++) {

    args[i].vetor = vetor;
    args[i].limite_inferior = i * fatia;

    if (i == num_threads - 1)
      args[i].limite_superior = tamanho;
    else
      args[i].limite_superior = (i + 1) * fatia;

    pthread_create(&threads[i], NULL, encontra_maximo_local, &args[i]);
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  int maximo_global = args[0].maximo_local;

  for (int i = 1; i < num_threads; i++) {
    if (args[i].maximo_local > maximo_global) {
      maximo_global = args[i].maximo_local;
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end);

  double tempo_gasto = calcula_tempo(start, end);

  printf("Maior elemento (Paralelo): %d\n", maximo_global);
  printf("Tempo Paralelo: %lf ms\n", tempo_gasto);

  free(vetor);
  free(threads);
  free(args);

  return 0;
}