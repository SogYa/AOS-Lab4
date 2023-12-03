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
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    int pid, wfd, rfd, rb, wb, i = 0;
    char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    struct flock lock;
    if (!fork())
    {
        if ((wfd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
        {
            printf("File opening failed wfd\n");
            exit(1);
        }

        while (i++ < 100)
        {
            if ((wb = write(wfd, digits, sizeof(digits))) < 0)
            {
                perror("Write error");
                kill(getppid(), SIGKILL);
                exit(1);
            }
        }

        close(wfd);
        exit(0);
    }
    else
    {
        if((rfd = open(argv[1], O_RDONLY)) < 0)
        {
            printf("File opening failed rfd\n");
            exit(1);
        }
        char buf1[1024];

        while ((rb = read(rfd, buf1, sizeof(buf1))) > 0)
        {
            sleep(1);
            if (rb == -1)
            {
                perror("Read error");
                kill(pid, SIGKILL);
                exit(1);
            }
            write(1, buf1, rb);
        }

        close(rfd);
        wait(NULL);
        exit(0);
    }
}