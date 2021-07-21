#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;
int fuel = 0;


/*
    Varios vehiculos desean cargar combustible (sustraer de la estacion)
    al mismo tiempo en el que la estacion esta reponiendo su combustible.

    Se reemplaza:
        pthread_cond_signal(&condFuel);        //solo avisa a un hilo 
    por
        pthread_cond_broadcast(&condFuel);     //avisa a todos los hilos



*/


//reponiendo combustible a la estacion de servicio
void* fuel_filling(void* arg) {

    //seccion critica
    for (int i = 0; i < 5; i++) {
        
        pthread_mutex_lock(&mutexFuel);         //accede y bloquea acceso a recursos 
        fuel += 30;                             //repone combustible en la estacion
        printf("Filled fuel... %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);       //desbloquea acceso a recursos

        pthread_cond_broadcast(&condFuel);      //avisar a todos los hilos
        
        sleep(1);

    }
}


//tomando combustible de la estacion de servicio
void* car(void* arg) {
    
    pthread_mutex_lock(&mutexFuel);
    
    while (fuel < 40) { //miestras no haya suficiente combustible para "servirse"
        printf("No fuel. Waiting...\n");
        pthread_cond_wait(&condFuel, &mutexFuel); //desbloquea, espera senal (combustible disponible) y vuelve a bloquear
        
        /* 
         Equivalente a...
         pthread_mutex_unlock(&mutexFuel);
         wait for signal on condFuel
         pthread_mutex_lock(&mutexFuel);
        */

    }

    fuel -= 40; //vehiculo toma 40 de combustible
    printf("Got fuel. Now left: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char* argv[]) {
    pthread_t th[6];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&condFuel, NULL);
    
    for (int i = 0; i < 6; i++) {
        if (i == 4 || i == 5) { //dos estaciones
            
            if (pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0) {
                perror("Failed to create thread");
            }

        } else { //cuatro autos
            
            if (pthread_create(&th[i], NULL, &car, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }

    //movilizando automoviles
    for (int i = 0; i < 6; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    //destruyendo mutex y vaariable de condicion
    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);
    return 0;
}