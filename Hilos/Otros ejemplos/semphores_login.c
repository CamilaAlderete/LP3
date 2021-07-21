#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 16
/*
    16 usuarios desean hacer login en una pagina
    La cantidad soportada de usuarios en paralelo es 10
    A medida que los usuarios cierren sesion, van a ir ingresando
        los usuarios restantes
*/
sem_t semaphore;

void* routine(void* args) {
    printf("(%d) Waiting in the login queue\n", *(int*)args);
    //ingresaran 10 usurios 
    sem_wait(&semaphore); //valor decrementa con cada usuario que ingresa 
    printf("(%d) Logged in\n", *(int*)args);
    sleep(rand() % 5 + 1); //estaran un tiempo aleatorio luego cerraran sesion
    printf("(%d) Logged out\n", *(int*)args);
    sem_post(&semaphore);// valor aumenta con cada usuario que sale
    free(args);

}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    sem_init(&semaphore, 0, 10);
    int i;


    for (i = 0; i < THREAD_NUM; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create thread");
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semaphore);
    return 0;
} 