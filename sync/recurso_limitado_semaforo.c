#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // Para sleep

#define NUM_THREADS 10
#define MAX_IMPRESSORAS_CONCORRENTES 3

sem_t semaforo_impressora;

void *trabalho_impressora(void *arg) {
    int thread_id = *(int *)arg;
    printf("Thread %d esperando para acessar a impressora...\n", thread_id);

    sem_wait(&semaforo_impressora);
    printf("Thread %d está imprimindo agora...\n", thread_id);
    sleep(2); // Simula o tempo de impressão
    printf("Thread %d terminou de imprimir.\n", thread_id);
    sem_post(&semaforo_impressora);


    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids_threads[NUM_THREADS];

    // Inicializa o semáforo com o número máximo de impressoras concorrentes
    sem_init(&semaforo_impressora, 0, MAX_IMPRESSORAS_CONCORRENTES);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids_threads[i] = i;
        pthread_create(&threads[i], NULL, trabalho_impressora, &ids_threads[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaforo_impressora);
    return 0;
}
