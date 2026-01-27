#include <stdio.h>
#include <unistd.h>

int main() {
    int f;

    f = fork();

    if (f == 0) {
        // Proceso hijo
        execl("./helloworld", (char*)NULL);
    } else {
        // Proceso padre
        printf("%d\n", (int)getpid());
        execl("./helloworld", (char*)NULL);
    }

    return 0;
}
