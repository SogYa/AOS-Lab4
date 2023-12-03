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
    int pipefd1[2], pipefd2[2], b = 0;
    pipe(pipefd1);
    pipe(pipefd2);
    char buf[512] = "\0";

    if (!fork())
    {
        close(pipefd1[1]);
        close(pipefd2[0]);
        printf("Дочерний процесс %d\n", getpid());
        dup2(pipefd2[1], 1);
        execl("task4_child", "task4_child", NULL);
        exit(EXIT_FAILURE);
    }
    else
    {
        close(pipefd1[0]);
        dup2(1, pipefd2[1]);
        while ((b = read(pipefd2[0], buf, 80)) > 0)
        {
            printf("PARENT read %d byte: %s\n", b, buf);
        }
        close(pipefd1[1]);
        close(pipefd2[0]);
        exit(0);
    }

    return 0;
}