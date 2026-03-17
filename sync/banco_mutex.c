#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Aumentamos as iterações para tornar as condições de corrida mais prováveis
#define ITERACOES 1000000
#define VALOR_ESPERADO 0 

// O patrimônio total do banco é o recurso compartilhado (seção crítica)
long long patrimonio_total_banco = 0;
pthread_mutex_t mutex_banco;

// Cada usuário tem seu próprio dinheiro e sua própria semente para números aleatórios
typedef struct {
    int id;
    long long saldo_pessoal;
    unsigned int semente;
} Usuario;

void *depositar(void *arg) {
    Usuario *u = (Usuario *)arg;
    for (int i = 0; i < ITERACOES; i++) {
        // rand_r é thread-safe e determinístico para uma mesma semente
        int valor = (rand_r(&u->semente) % 100) + 1;
        u->saldo_pessoal += valor;

        pthread_mutex_lock(&mutex_banco);
        patrimonio_total_banco += valor;
        pthread_mutex_unlock(&mutex_banco);
    }
    return NULL;
}

void *sacar(void *arg) {
    Usuario *u = (Usuario *)arg;
    for (int i = 0; i < ITERACOES; i++) {
        int valor = (rand_r(&u->semente) % 100) + 1;
        
        pthread_mutex_lock(&mutex_banco);
        patrimonio_total_banco -= valor;
        pthread_mutex_unlock(&mutex_banco);

        u->saldo_pessoal -= valor;
    }
    return NULL;
}

int main() {
    pthread_t tid[4];
    Usuario usuarios[4];

    pthread_mutex_init(&mutex_banco, NULL);
    // Sementes emparelhadas: 
    // Usuário 0 (Depósito) e Usuário 2 (Saque) usam semente 42.
    // Usuário 1 (Depósito) e Usuário 3 (Saque) usam semente 123.
    // Como a sequência de valores será a mesma, o saldo final DEVE ser 0.
    unsigned int sementes[] = {42, 123, 42, 123};

    for (int i = 0; i < 4; i++) {
        usuarios[i].id = i + 1;
        usuarios[i].semente = sementes[i];
        // Inicializamos com um valor alto para evitar "saldos negativos" puramente ilustrativos
        usuarios[i].saldo_pessoal = (i >= 2) ? 100000000 : 0; 
    }

    // Criando threads: 2 depositantes e 2 sacadores
    pthread_create(&tid[0], NULL, depositar, &usuarios[0]);
    pthread_create(&tid[1], NULL, depositar, &usuarios[1]);
    pthread_create(&tid[2], NULL, sacar, &usuarios[2]);
    pthread_create(&tid[3], NULL, sacar, &usuarios[3]);

    for (int i = 0; i < 4; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("--- Verificação de Sincronização ---\n");
    printf("Patrimônio Total do Banco: %lld\n", patrimonio_total_banco);
    printf("Valor Esperado: %d\n", VALOR_ESPERADO);

    if (patrimonio_total_banco == VALOR_ESPERADO) {
        printf("\nRESULTADO: SUCESSO! (O sistema está sincronizado)\n");
    } else {
        printf("\nRESULTADO: FALHA! (Condição de corrida detectada)\n");
        printf("Erro acumulado: %lld\n", patrimonio_total_banco - VALOR_ESPERADO);
    }

    pthread_mutex_destroy(&mutex_banco);
    return 0;
  }
