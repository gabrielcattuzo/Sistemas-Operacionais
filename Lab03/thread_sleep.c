#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *dorme(void *arg) {
    long id = (long)arg;
    printf("Thread %ld dormindo...\n", id);
    sleep(60); // Dorme por sessenta segundos para dar tempo de inspecionar no terminal
    printf("Thread %ld acordou!\n", id);
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2, t3;

    printf("Criando 3 threads. Meu PID eh: %d\n", getpid());

    pthread_create(&t1, NULL, dorme, (void *)1);
    pthread_create(&t2, NULL, dorme, (void *)2);
    pthread_create(&t3, NULL, dorme, (void *)3);

    // Espera as threads terminarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Fim do programa principal!\n");
    return 0;
}
