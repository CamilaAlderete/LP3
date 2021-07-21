#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define THREAD_NUM 8

/*
    Un hilo A utiliza un recurso M y un hilo B utiliza un
    recurso N. El deadlock ocurre cuando ambos hilos, para continuar con su ejecucion 
    necesitan los recursos del otro.
    El hilo A necesita de N (ocupado por B) y B necesita de M (ocupado por A).
    Ningun hilo cedera su recurso por lo que ambos se quedaran esperando eternamente...

*/

pthread_mutex_t mutexFuel;
int fuel = 50;
pthread_mutex_t mutexWater;
int water = 10;


//varios hilos realizan la rutina
void* routine(void* args) {

    //posibles problemas en las sgtes lineas....

    if (rand() % 2 == 0) { //algunos hilos ingresaran aqui
        pthread_mutex_lock(&mutexFuel); // 1. el hilo A accede al recurso fuel
        sleep(1);
        pthread_mutex_lock(&mutexWater); // 2. el hilo A espera, ya que water esta ocupado por B


    } else { //otros hilos ingresaran aqui
        pthread_mutex_lock(&mutexWater);// 1. el hilo B accede al recurso water
        sleep(1);
        pthread_mutex_lock(&mutexFuel); // 2. el hilo B espera porque fuel esta ocupado por A
    }

    /*si ocurre lo anteriormente comentado, A Y B esperaran eternamente,
      ya que ninguno cedara sus recursos y no pueden continuar sin estos...
    */
    fuel += 50;
    water = fuel;
    printf("Incremented fuel to: %d set water to %d\n", fuel, water);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexWater);
}

int main(int argc, char *argv[]) {

    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_mutex_init(&mutexWater, NULL);

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
    printf("Water: %d\n", water);
    pthread_mutex_destroy(&mutexFuel);
    pthread_mutex_destroy(&mutexWater);
    return 0;
}