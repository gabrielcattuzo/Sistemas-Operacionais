#include <stdio.h>
#include <stdlib.h>

/* 
 * Este programa lê uma string como primeiro argumento e a escreve no arquivo passado no segundo argumento!
 */

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Uso: %s string_entrada arquivo_saida", argv[0]);
    return 1;
  }

  FILE *f = fopen(argv[2], "w");

  if (f == NULL) {
    fprintf(stderr, "Não consegui abrir o arquivo de saída!\n");
    return 1;
  }

  fprintf(f, "%s\n", argv[1]);
  fclose(f);
  return 0;
}
