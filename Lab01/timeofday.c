#include <stdio.h>
#include <sys/syscall.h> 
#include <sys/time.h>    
#include <unistd.h>      

#define ITERACOES 1000000

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Uso: %s <l|s>\n"
                    "\tl => libc (vDSO)\n"
                    "\ts => syscall (Raw)\n",
            argv[0]);
    return 1;
  }

  struct timeval tv;
  char opcao = argv[1][0];

  if (opcao == 'l') {
    for (int i = 0; i < ITERACOES; i++)
        gettimeofday(&tv, NULL);

    printf("Glibc (vDSO): %ld sec, %ld usec (Ultima iteracao)\n", tv.tv_sec, tv.tv_usec);
  } else if (opcao == 's') {
    for (int i = 0; i < ITERACOES; i++) 
        syscall(SYS_gettimeofday, &tv, NULL);

    printf("Syscall (Raw): %ld sec, %ld usec (Ultima iteracao)\n", tv.tv_sec, tv.tv_usec);
  } else {
    fprintf(stderr, "Opcao invalida!\n");
    return 1;
  }

  return 0;
}
