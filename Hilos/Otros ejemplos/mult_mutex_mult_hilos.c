#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

// chefs = threads
// stove = shared data (+mutex)

/*
    - Existen varios hornos (recursos compartidos) y varios chefs (hilos) donde cada chef puede usar un horno.
    - Cada chef (itera) busca un horno libre, una vez que encuentra uno libre, cocina en el y al terminar se marcha
    - Si se ha iterado por todos los hornos y ninguno esta libre, el chef espera varios segundos y luego 
        comienza de nuevo la iteracion en busqueda de un horno libre. 

*/

pthread_mutex_t stoveMutex[4];

int stoveFuel[4] = { 100, 100, 100, 100 }; //combustible disponible en el horno

void* routine(void* args) {
    //cuatro hornos
    for (int i = 0; i < 4; i++) {

        //con trylock si el recurso no esta disponible, no espera (cola?) para acceder, hace otra cosa 
        if (pthread_mutex_trylock(&stoveMutex[i]) == 0) { 
            //si el horno esta libre    

            int fuelNeeded = (rand() % 30); //combustible que necesita el chef
            
            if (stoveFuel[i] - fuelNeeded < 0) { //horno sin combustible suficiente
                printf("No more fuel... going home\n");

            } else { //horno con combustible suficiente para cocinar

                stoveFuel[i] -= fuelNeeded;
                usleep(500000);
                printf("Fuel left %d\n", stoveFuel[i]);
            }

            pthread_mutex_unlock(&stoveMutex[i]); //libera recurso
            break; //sale del ciclo for...

        } else {// horno ocupado

            if (i == 3) { //ultima iteracion, no se encontro horno libre
                printf("No stove available yet, waiting...\n");
                usleep(300000);//esperar...
                i = 0; //reiniciar busqueda de horno libre
            }
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    // 10 chefs
    pthread_t th[10]; 

    //cuatro hornos
    for (int i = 0; i < 4; i++) { 
        pthread_mutex_init(&stoveMutex[i], NULL);
    }


    for (int i = 0; i < 10; i++) {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("Failed to create thread");
        }
    }


    // "movilizando a los chefs"
    for (int i = 0; i < 10; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    //destruyendo los mutex
    for (int i = 0; i < 4; i++) {
        pthread_mutex_destroy(&stoveMutex[i]);
    }

    return 0;
}