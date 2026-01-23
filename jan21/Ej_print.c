#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Hello CC3064!\n");
    printf("%d\n", (int)getpid());
    return 0;
}