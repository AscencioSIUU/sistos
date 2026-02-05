#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NTHREADS 100

void *say_hello(void *threadNumber) {
    intptr_t tID;
    tID = (intptr_t) threadNumber;
    printf("Hello thread: %ld\n", tID);
    pthread_exit(NULL);
    return NULL;
}

int main() {
    int rc;
    intptr_t i;
    pthread_t tid[NTHREADS];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (i = 0; i < NTHREADS; i++) {
        rc = pthread_create(&tid[i], &attr, say_hello, (void *) i);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }

    }

    for (i = 0; i < NTHREADS; i++) {
        rc = pthread_join(tid[i], NULL);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    return 0;
}
