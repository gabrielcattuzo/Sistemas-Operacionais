#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// funções ajudantes
double calcula_tempo(struct timespec start, struct timespec end) {
  return (end.tv_sec - start.tv_sec) * 1000 + (double) (end.tv_nsec - start.tv_nsec)/1000000; 
}

void compara_tempos(double tempo_seq, double tempo_paral) {
  if (tempo_seq <= tempo_paral) 
    printf("👎 Versão paralela é %lf vezes mais lenta que a sequencial\n", tempo_paral/tempo_seq);
  else
    printf("👍 Versão paralela é %lf vezes mais rápida que a sequencial\n", tempo_seq/tempo_paral);
}

typedef struct {
  long limite_inferior;
  long limite_superior;
  long soma_local;
  pthread_t thread_id;
} parametros;

void *soma(void *param) {
  long sum = 0;
  parametros *p = (parametros *) param;

  char* should_print = getenv("PRINT_THREADS");
  if (should_print != NULL)
    printf("Iniciando soma da thread %lu\n", p->thread_id);
  for (long i = p->limite_inferior ; i <= p->limite_superior ; ++i)
    sum += i;

  if (should_print != NULL)
    printf("Thread %lu concluiu seu trabalho!\n", p->thread_id);

  p->soma_local = sum;
  pthread_exit(0);
}

long soma_sequencial(long num) {
  long soma = 0;
  for (long i = 1 ; i <= num ; ++i)
    soma += i;
  return soma;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <num_threads> <sum_limit>\n", argv[0]);
    return 1;
  }

  int num_threads = atoi(argv[1]);
  long num_iter = atol(argv[2]);

  pthread_t *thread_ids = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
  parametros *param = (parametros *) malloc(sizeof(parametros) * num_threads);

  struct timespec start, end;
  for (int i = 0 ; i < num_threads ; ++i)  {
    // limite inferior para essa thread
    param[i].limite_inferior = i * num_iter / num_threads;
    // limite superior para essa thread
    if (i != num_threads - 1) 
      param[i].limite_superior = (i + 1) * num_iter / num_threads - 1;
    else 
      param[i].limite_superior  = num_iter;

    // numero da thread
    param[i].thread_id = i;
    param[i].soma_local = 0;

  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int i = 0 ; i < num_threads ; ++i)
    pthread_create(&thread_ids[i], NULL, soma, &param[i]);

  for (int i = 0 ; i < num_threads ; ++i)
    pthread_join(thread_ids[i], NULL);
  clock_gettime(CLOCK_MONOTONIC, &end);

  double tempo_paralelo = calcula_tempo(start, end);
  long sum = 0;
  for (int i = 0 ; i < num_threads ; ++i)
    sum += param[i].soma_local;
  printf("paralelo -> %ld | %lf ms\n", sum, tempo_paralelo);

  free(thread_ids);
  free(param);

  clock_gettime(CLOCK_MONOTONIC, &start);
  long sum_seq = soma_sequencial(num_iter);
  clock_gettime(CLOCK_MONOTONIC, &end);
  double tempo_seq = calcula_tempo(start, end);
  printf("sequencial -> %ld | %lf ms\n", sum_seq, tempo_seq);

  // comparação de tempo
  compara_tempos(tempo_seq, tempo_paralelo); 

  if (sum_seq != sum) {
    printf("❌ Versão paralela produz resultado incorreto!\n");
  }

  return 0;
}
