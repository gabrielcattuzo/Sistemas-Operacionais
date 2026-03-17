#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    const int SIZE = 4096;
    const char *name = "/prod_cons";

    int shm_fd;
    void *ptr;

    /* cria memória compartilhada */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);

    /* mapeia memória */
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    pid_t pid = fork();

    if (pid == 0) {

        /* FILHO = PRODUTOR */
        sprintf(ptr, "Hello from shared memory!\n");

    } else {

        /* PAI = CONSUMIDOR */
        wait(NULL);

        printf("Consumidor leu: %s", (char *)ptr);

        shm_unlink(name);
    }

    return 0;
}