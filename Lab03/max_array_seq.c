#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para calcular o tempo percorrido
double calcula_tempo(struct timespec start, struct timespec end) {
  return (end.tv_sec - start.tv_sec) * 1000.0 + (double)(end.tv_nsec - start.tv_nsec) / 1000000.0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <tamanho_vetor>\n", argv[0]);
        return 1;
    }

    long tamanho = atol(argv[1]);

    // Aloca o vetor
    int *vetor = (int *)malloc(tamanho * sizeof(int));
    if (vetor == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para o vetor\n");
        return 1;
    }

    // Inicializa o vetor com valores pseudo-aleatorios
    // Garantindo um valor maximo conhecido para teste
    srand(time(NULL));
    for (long i = 0; i < tamanho; i++) {
        vetor[i] = rand() % 1000000;
    }
    // Planta um valor sabidamente máximo no meio
    vetor[tamanho / 2] = 2000000;

    struct timespec start, end;
    
    // Inicia a medicao de tempo
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Encontra o maximo sequencialmente
    int maximo = vetor[0];
    for (long i = 1; i < tamanho; i++) {
        if (vetor[i] > maximo) {
            maximo = vetor[i];
        }
    }

    // Fim da medicao
    clock_gettime(CLOCK_MONOTONIC, &end);

    double tempo_gasto = calcula_tempo(start, end);

    printf("Maior elemento (Sequencial): %d\n", maximo);
    printf("Tempo Sequencial: %lf ms\n", tempo_gasto);

    free(vetor);
    return 0;
}
