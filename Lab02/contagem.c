#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    pid_t pid;

    for (int i = 0; i < 4; i++) 
    {
        pid = fork();

        if (pid == 0) 
      {
            break;
      }
    }

    printf("Hello, World!\n");

    return 0;
}