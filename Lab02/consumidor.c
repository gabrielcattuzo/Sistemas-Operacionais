#include <stdio.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main() {
    const int SIZE = 4096;
    const char *name = "/prod_cons";

    int shm_fd;
    void *ptr;

    /* abre o objeto de memória compartilhada */
    shm_fd = shm_open(name, O_RDONLY, 0666);

    if (shm_fd == -1) {
      fprintf(stderr, "Não existe memória compartilhada com nome de %s\n", name);
      return 1;
    }

    /* mapeia a memória compartilhada */
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    /* lê do objeto de memória compartilhada */
    printf("Consumidor leu: %s", (char *)ptr);

    /* remove o objeto de memória compartilhada */
    shm_unlink(name);

    return 0;
}
