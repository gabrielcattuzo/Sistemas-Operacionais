#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    const int SIZE = 4096;
    const char *name = "/prod_cons";
    const char *message_0 = "Hello";
    const char *message_1 = " World!";

    int shm_fd;
    void *ptr;

    /* cria objeto de memória compartilhada */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);

    /* mapeamento do objeto para a memória */
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    /* escreve na memória compartilhada */
    sprintf(ptr, "%s", message_0);
    ptr += strlen(message_0);
    sprintf(ptr, "%s", message_1);
    ptr += strlen(message_1);
    sprintf(ptr, "\n");
    ptr += 1;

    printf("Produtor: Dados escritos na memória.\n");

    return 0;
}
