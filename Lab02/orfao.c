#include <stdio.h>
#include <unistd.h>

int main() {

    pid_t pid = fork();

    if (pid > 0) {
        printf("Processo pai finalizando...\n");
    }
    else if (pid == 0) {
        sleep(5);
        printf("Sou o processo filho e agora sou órfão!\n");
        printf("Meu novo pai eh o PID: %d\n", getppid());
    }

    return 0;
}