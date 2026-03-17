#include <stdio.h>
#include <unistd.h>

int main() {

    pid_t pid = fork();

    if (pid > 0) {
        sleep(10);
        printf("Processo pai acordou.\n");
    }
    else if (pid == 0) {
        printf("Processo filho finalizando.\n");
    }

    return 0;
}