#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <time.h>

int main()
{
    int fd[2];
    pipe(fd);

    if (!fork())
    {
        close(fd[0]);
        printf("Дочерний процесс %d\n", getpid());

        dup2(fd[1], STDOUT_FILENO);
        execlp("who", "who", NULL);
    }
    else
    {
        close(fd[1]);
        printf("Родитлеьский %d\n", getpid());
        dup2(STDOUT_FILENO, fd[1]);
        dup2(fd[0], STDIN_FILENO);
        execlp("wc", "wc", "-l", NULL);
    }

    return 0;
}