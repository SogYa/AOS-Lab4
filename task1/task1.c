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
    int pipefd1[2], pipefd2[2];

    pipe(pipefd1);
    pipe(pipefd2);

    if (!fork()) {
        close(pipefd1[1]);
        close(pipefd2[0]);

        char phrase[1024] = {0};
        read(pipefd1[0], phrase, 1024);
        printf("Дочерний процесс: %s\n", phrase);

        const char* message = "Передача из дочернего процесса";
        write(pipefd2[1], message, strlen(message));
        close(pipefd1[0]);
        close(pipefd2[1]);
        
    } else {
        close(pipefd1[0]);
        close(pipefd2[1]);
        const char* message = "Передача из родительского процесса";
        write(pipefd1[1], message, strlen(message));

        char reply[1024] = {0};
        read(pipefd2[0], reply, 1024);
        printf("Родительский процесс: %s\n", reply);
        close(pipefd1[1]);
        close(pipefd2[0]);
        
    }

    return 0;
}