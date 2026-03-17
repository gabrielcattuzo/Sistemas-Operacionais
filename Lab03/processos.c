#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int valor = 0; // Variavel global

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Erro ao criar processo.\n");
        return 1;
    } else if (pid == 0) {
        // Processo filho
        valor = 10;
        printf("Filho: alterei o valor para %d\n", valor);
    } else {
        // Processo pai
        wait(NULL); // Aguarda o filho terminar
        printf("Pai: o valor final eh %d\n", valor);
    }

    return 0;
}
