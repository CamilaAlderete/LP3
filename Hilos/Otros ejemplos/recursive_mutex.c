#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define THREAD_NUM 8

pthread_mutex_t mutexFuel;
int fuel = 50;

void* routine(void* args) {

    /*  Si se bloquea n veces, se debe desbloquear n veces...
        esto solo puede realizarlo un hilo a la vez.
        
        Una vez que el hilo lo haya desbloqueado n veces,
        el resto de los hilos pueden tener oportunidad de acceder al recurso.
    */
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    fuel += 50;
    printf("Incremented fuel to %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char *argv[]) {

    pthread_t th[THREAD_NUM];

    //configuracion del mutex
    pthread_mutexattr_t recursiveMutexAttributes; //atributos del mutex, no es el mutex
    pthread_mutexattr_init(&recursiveMutexAttributes);
    pthread_mutexattr_settype(&recursiveMutexAttributes, PTHREAD_MUTEX_RECURSIVE);

    //establecer configuracion
    pthread_mutex_init(&mutexFuel, &recursiveMutexAttributes);


    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("Failed to create thread");
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    printf("Fuel: %d\n", fuel);

    pthread_mutexattr_destroy(&recursiveMutexAttributes);
    pthread_mutex_destroy(&mutexFuel);
    
    return 0;
}