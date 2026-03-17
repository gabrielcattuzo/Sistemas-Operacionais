#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define TAMANHO_BUFFER 10

int buffer[TAMANHO_BUFFER];
int indice = -1;

pthread_mutex_t mutex;
sem_t vazio;  // Conta slots vazios
sem_t cheio;  // Conta slots cheios

void *produtor(void *arg) {
    int item = 0;
    for (int i = 0; i < 20; i++) { // Produz 20 itens
        item = i;
        sem_wait(&vazio); // Espera por um slot vazio
        pthread_mutex_lock(&mutex); // Bloqueia o mutex para acessar o buffer

        buffer[++indice] = item;
        printf("Produtor produziu: %d no indice %d\n", item, indice);

        pthread_mutex_unlock(&mutex); // Libera o mutex
        sem_post(&cheio);  // Sinaliza que um slot está cheio
    }
    return NULL;
}

void *consumidor(void *arg) {
    int item;
    for (int i = 0; i < 20; i++) { // Consome 20 itens
        sem_wait(&cheio); // Espera por um slot cheio
        pthread_mutex_lock(&mutex); // Bloqueia o mutex para acessar o buffer

        item = buffer[indice--];
        printf("Consumidor consumiu: %d do indice %d\n", item, indice + 1);

        pthread_mutex_unlock(&mutex); // Libera o mutex
        sem_post(&vazio); // Sinaliza que um slot está vazio
    }
    return NULL;
}

int main() {
    pthread_t thread_prod, thread_cons;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&vazio, 0, TAMANHO_BUFFER); // Inicialmente, todos os slots estão vazios
    sem_init(&cheio, 0, 0);               // Inicialmente, nenhum slot está cheio

    pthread_create(&thread_prod, NULL, produtor, NULL);
    pthread_create(&thread_cons, NULL, consumidor, NULL);

    pthread_join(thread_prod, NULL);
    pthread_join(thread_cons, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&vazio);
    sem_destroy(&cheio);

    return 0;
}
