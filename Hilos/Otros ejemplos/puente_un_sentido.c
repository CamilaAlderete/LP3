#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h> //boleano

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;

int puente[3]={0};

void* ver(){
    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            printf("%5d",puente[i]);
        }
        printf("\n");
        sleep(1);
        system("clear");
        
    }
    
}

void* f(void* arg){
    int* i = (int*) arg;
    pthread_mutex_lock(&mutex1);
        //printf("1. holaa de parte de %d\n",*i);
        puente[0]=*i;
        sleep(1);
        puente[0]=0;
    pthread_mutex_unlock(&mutex1);

    pthread_mutex_lock(&mutex2);
        //printf("2. holaa de parte de %d\n",*i);
        puente[1]=*i;
        sleep(1);
        puente[1]=0;
    pthread_mutex_unlock(&mutex2);

    pthread_mutex_lock(&mutex3);
        //printf("3. holaa de parte de %d\n",*i);
        puente[2]=*i;
        sleep(1);
        puente[2]=0;
    pthread_mutex_unlock(&mutex3);
}

void* robar(void* arg){
    //retrendra por unos segundos el trafico en el bloque 3 del puente
    sleep(2);
    pthread_mutex_lock(&mutex3);
        sleep(2);
    pthread_mutex_unlock(&mutex3);
}

int main(int argc, char const *argv[])
{
    pthread_t hilos[5];
	pthread_mutex_init(&mutex1, NULL); // inicializar la referencia a mutex
	pthread_mutex_init(&mutex2, NULL); // inicializar la referencia a mutex
	pthread_mutex_init(&mutex3, NULL); // inicializar la referencia a mutex

    pthread_t vista;
    pthread_create(&vista, NULL, &ver,NULL);
    

    //este hilo  retrendra por unos segundos el trafic en el bloque 3 del puente
    pthread_t hiloLadron;
    pthread_create(&hiloLadron, NULL, &robar,NULL);


    for (int i = 0; i < 5; i++)
    {
        //puts("...");
        int* j = (int *)malloc(sizeof(int*));
        *j=i;

        //puts("111");
        if( pthread_create(&hilos[i], NULL, &f, (void*)j) !=0 ){
            perror("create");
        }    
        //puts(".........");
    }



    for (int i = 0; i <5; i++)
    {
        pthread_join(hilos[i],NULL);
    }

    pthread_join(vista,NULL); //con esto el hilo nunca termina... lo cual esta  bien
    
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_mutex_destroy(&mutex3);




    return 0;
}
