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
    int fd[2], fd_rev[2];

    pipe2(fd, O_NONBLOCK);
    pipe2(fd_rev, O_NONBLOCK);

    if (!fork())
    {
        close(fd[0]);
        close(fd_rev[1]);
        int r;
        do
        {
            char buf[80] = {0};
            r = read(STDIN_FILENO, buf, 80);
            if (r > 0)
            {
                printf("Дочерний процесс: %s", buf);
                write(fd_rev[1], buf, r);
            }
        } while (r);
        close(fd[1]);
        close(fd_rev[0]);
    }
    else
    {
        close(fd[1]);
        close(fd_rev[0]);
        int r;

        do
        {
            char buf[80] = {0};
            r = read(STDIN_FILENO, buf, 80);
            if (r > 0)
            {
                printf("Родительский процесс: %s", buf);
                write(fd_rev[1], buf, r);
            }
        } while (r);
        close(fd[0]);
        close(fd_rev[1]);
    }

    return 0;
}