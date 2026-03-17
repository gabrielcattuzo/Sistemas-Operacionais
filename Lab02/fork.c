#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  pid_t pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Erro ao criar um novo processo!\n");
    return 1;
  }

  if (pid > 0) {
    printf("Sou o processo pai!\n");
    printf("Criei um filho com PID %d\n", pid);
  }
  else {
    printf("Sou o processo filho!\n");
  }

  return 0;
}
