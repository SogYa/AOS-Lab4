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


int main() {
    //Через файл
    int fd[2];
    pipe(fd);

    if (!fork()) {
        close(fd[0]);
        int n;

        while(1) {
            char buf[80] = {0};
            n = read(STDIN_FILENO, buf, 80);
            write(fd[1], buf, n);
        }

    } else {
        close(fd[1]);
        int r;

        do {
            char buf[80] = {0};
            r = read(fd[0], buf, 80);
            printf("Полученное сообщение: %s", buf);
        } while(r);
        close(fd[0]);
        close(fd[1]);
    }

    return 0;
}