#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int b = 0;
    char buf[512];

    b = read(STDIN_FILENO, buf, sizeof(buf));
    write(1, buf, b);

    return 0;
}