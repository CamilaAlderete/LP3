#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

#define N_HILOS 5

int main(int argc, char const *argv[])
{
    
    pthread_t hilos[N_HILOS]; //hilos
    pthread_attr_t tattr; //atributos del hilo
    struct sched_param param; //parametros para prioridad (scheduling)

    int prioridad = 1;

    //inicializar con atributos por defecto
    if (pthread_attr_init (&tattr)!=0)
    {
        perror("pthread_attr_init");
    }

    if (pthread_attr_getschedparam (&tattr, &param) !=0)
    {
        perror("pthread_attr_getschedparam");
    }
    
    

    //configurar la prioridad de los hilos...
    for (int i = 0; i < N_HILOS; i++)
    {
        
    }
    




    return 0;
}
