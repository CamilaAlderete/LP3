#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;

int fuel = 0; //recurso

/*
    Un vehiculo desea cargar combustible (sustraer de la estacion)
    al mismo tiempo en el que la estacion esta reponiendo su combustible.

    Esta implementacion solo sirve con una estacion y un solo vehiculo.
    Surgen inconvenientes cuando se tienen mas de un vehiculo.

*/


//reponiendo combustible a la estacion de servicio
void* fuel_filling(void* arg) {
    for (int i = 0; i < 5; i++) {

        //seccion critica
        pthread_mutex_lock(&mutexFuel);
        fuel += 15;
        printf("Filled fuel... %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);

        //luego de finalizar el acceso al recurso, envia senal 
        pthread_cond_signal(&condFuel); //signal solo "despierta a un hilo" y es al que tenga mas prioridad??, con mas hilos usar otra funcion
        
        sleep(1);
    }
}

//tomando combustible de la estacion de servicio
void* car(void* arg) {

    //seccion critica
    pthread_mutex_lock(&mutexFuel);
    while (fuel < 40) {
        printf("No fuel. Waiting...\n");

        //desbloquea el mutex, espera senal y luego bloquea de nuevo el mutex
        pthread_cond_wait(&condFuel, &mutexFuel); //duerme el hilo hasta recibir senal 
        
        /* Equivalente a...
         pthread_mutex_unlock(&mutexFuel);
         wait for signal on condFuel
         pthread_mutex_lock(&mutexFuel);*/
    }

    fuel -= 40;
    printf("Got fuel. Now left: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);

}

int main(int argc, char* argv[]) {
    pthread_t th[2];
    //mutex
    pthread_mutex_init(&mutexFuel, NULL);
    //variable de condicion
    pthread_cond_init(&condFuel, NULL);

    for (int i = 0; i < 2; i++) {

        if (i == 1) {//el primer hilo se encarga de cargar combustible
            if (pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {//el segundo hilo del auto
            if (pthread_create(&th[i], NULL, &car, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }

    //ejecutar hilos
    for (int i = 0; i < 2; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    //destruir mutex y variable de condicion
    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);
    return 0;
}