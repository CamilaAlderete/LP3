#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

int contador = 0;
pthread_mutex_t mutex;

int total_hilos;

int turno = 0;


typedef struct Dato_del_hilo
{
    int n; //nro a calcular el factorial
    int turno;//turno de salida 
    long long resultado;//resultado del factorial
} dato_hilo;

void salida_ordenada_(dato_hilo *datos){

    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (turno == datos->turno) //si es el turno del hilo...
        {
            //impresion de los datos del hilo
            if (datos->resultado > -1)
            {
                printf("Factorial de %d: %llu\n", datos->n, datos->resultado);
            }else
            {
                printf("Factorial de %d: [no existe]\n",datos->n);
            }

    	    turno++;
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL); //chau

        }
        pthread_mutex_unlock(&mutex);

        
    }

}


//el factorial se calculara de forma concurrente y solo la salida sera secuencial
void* factorial(void* arg){

    dato_hilo *datos = (dato_hilo*)malloc(sizeof(dato_hilo*));
    datos = (dato_hilo*) arg;

    int n = datos->n;
    long long fact = 1;

    //calculo del factorial
    if (n < 0)
        datos->resultado = -1;
    else {
        for (int i = 1; i <= n; ++i) {
            fact *= i;
        }

        datos->resultado = fact;
    }

    salida_ordenada_(datos);

}

int main(int argc, char const *argv[])
{

    int *n;
    dato_hilo *datos;

    if (argc<2)
    {
      printf("Argumentos insuficientes\n");
      return -1;
    }

    //hilos
    total_hilos= argc-1;
    pthread_t t[total_hilos];

    //mutex para terminacion en secuencia
    pthread_mutex_init(&mutex, NULL); 
    
    //creacion de los hilos y ejecucion de rutina
    for (int i = 0; i < total_hilos; i++)
    {
      datos = (dato_hilo*)malloc(sizeof(dato_hilo*));
      datos->n = atoi( argv[i+1] );
      datos->turno = i;

       pthread_create(&t[i], NULL, &factorial,datos);

    }
 
    //para que terminen aqui
    for (int i = 0; i < total_hilos; i++)
    {
       pthread_join(t[i],NULL);

    }

    //destruccion de mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}

