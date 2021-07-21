#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 8

sem_t semaphore;

/*
    - Un semaforo funciona como una variable
        En este caso...
        - El semaforo tendra un valor 4 
        - Por cada hilo que acceda al recurso se decrementara una unidad 
            con sem_wait(&semaphore);
        - Cuando 4 hilos hayan accedido, el semaforo valdra 0
        - Cuando vale 0, ningun otro hilo puede acceder
        -Los 4 hilos realizan las intrucciones en paralelo
        -Cada hilo que deje de ocupar el recurso, hace
            que el semaforo aumente una unidad con sem_post(&semaphore);
        -El semaforo volvera a valer 4.
        -Los hilos restantes pueden volver a intentar acceder al recurso 
*/

/*
    Se puede obtener el valor de semaforo con sem_getvalue(&semaphore, &var)
    pero esta variable que se obtiene no esta exenta del race condition (condicion de carrera)
    por lo que no se puede esperar valores "coherentes"
*/

void* routine(void* args) {
    sem_wait(&semaphore);
    sleep(1);
    printf("Hello from thread %d\n", *(int*)args);
    sem_post(&semaphore);
    free(args);
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];

    //arg1: semaforo, arg2: nro de procesos, arg3: nro de hilos
    sem_init(&semaphore, 0, 4);

    int i;
    //creacion de hilos
    for (i = 0; i < THREAD_NUM; i++) {

        //enviara un argumento "a" a la funcion routine
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create thread");
        }
    }

    //ejecucion de hilos
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    //destruye semaforo
    sem_destroy(&semaphore);
    return 0;
}