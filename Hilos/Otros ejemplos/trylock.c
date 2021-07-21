#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t mutex;

/*
 CON LOCK, LOS HILOS QUE NO LOGRARON ACCEDER AL RECURSO "ESPERAN" 
 A QUE SEA LIBERADO. OSEA, LOS HILOS ACCEDERAN UNO POR UNO AL RECURSO (COMO UNA FILA DE ESPERA)
*/
void* routine_lock(void* arg) {
    pthread_mutex_lock(&mutex);
    printf("Got lock\n");
    sleep(1);
    pthread_mutex_unlock(&mutex);
}

/*
CON TRYLOCK, EL PRIMER HILO QUE INGRESA ACCEDE AL RECURSO Y EJECUTA LA SECCION CRITICA.
SI LLEGA OTRO HILO CON INTENCION DE ACCEDER AL RECURSO Y ESTE SIGUE OCUPADO,
**NO ESPERA A QUE SE DESOCUPE*** SINO QUE DIRECTAMENTE "HACE OTRA COSA".

UNA VEZ QUE EL RECURSO SE DESOCUPE, OTROS HILOS PUEDEN TENER ACCESO AL EL, PERO LOS HILOS QUE
PASARON MIENTRAS EL RECURSO ESTABA EN USO YA "NO TIENEN OPORTUNIDAD A ACCEDERLO"

 */
void* routine_trylock(void* arg) {
    if (pthread_mutex_trylock(&mutex) == 0) { //hilo accede al recurso
        printf("Got lock\n");
        sleep(1);
        pthread_mutex_unlock(&mutex);
    } else { //recurso ocupado, el hilo hace otra cosa y pierde oportunidad de acceso al recurso
        printf("Didn't get lock\n");
    }
}

int main(int argc, char* argv[]) {
    pthread_t th[4];
    pthread_mutex_init(&mutex, NULL);

    //SE CREAN 4 HILOS QUE EJECUTARAN UNA RUTINA
    for (int i = 0; i < 4; i++) {
        if (pthread_create(&th[i], NULL, &routine_trylock, NULL) != 0) {
            perror("Error at creating thread");
        }
    }

    //COMIENZA LA EJECUCION DE LA RUTINA EN LOS 4 HILOS
    for (int i = 0; i < 4; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Error at joining thread");
        }
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}