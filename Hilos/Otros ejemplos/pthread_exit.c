#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h> //sleep

void* roll_dice() {
    int value = (rand() % 6) + 1;
    int* result = malloc(sizeof(int));
    *result = value;
    sleep(2);
    printf("Thread result: %d\n", value);
    pthread_exit((void*) result); //retorna dato al terminar el hilo
}

int main(int argc, char* argv[]) {
    int* res;
    srand(time(NULL));
    pthread_t th;
    if (pthread_create(&th, NULL, &roll_dice, NULL) != 0) {
        return 1;
    }
    //pthread_exit(0); //se puede terminar el proceso principal, y el hilo continua su ejecucion
    if (pthread_join(th, (void**) &res) != 0) {
        return 2;
    }
    printf("Result: %d\n", *res);
    free(res);
    return 0;
}