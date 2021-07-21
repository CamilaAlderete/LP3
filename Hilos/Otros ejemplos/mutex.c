#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/* 
- Un proceso puede tener multiples hilos
- En un proceso, los hilos comparten memoria
- Los hilos son ejecutados en paralelo
- Si varios hilos a la vez desean modificar una variable
  se deben implementar tecnicas de sincronizacion para
  evitar problemas.
- Se emplea mutex para sincronizar a los hilos en la escritura
  en una variable.
  
  Observaciones...
  Si se desea crear N hilos en un ciclo, no hacer lo sgte.:

  	for (int i = 0; i < 4; i++)
	{

		//CREACION DE HILO
		if ( pthread_create(th+i, NULL, &routine, NULL)!= 0 )
		{
			perror("pthread_create: ");
			return 1;
		}


		//EJECUCION DE RUTINA
		if (pthread_join(th[i], NULL) !=0)
		{
		 	perror("pthread_join: ");
		 	return 2;
		}

	}

	ESTO PORVOCARA QUE LOS HILOS SE CREEN Y EJECUTEN SECUENCIALMENTE Y
	NO SE APROVECHA LA CARACTERISTICA DE PARALELISMO DE LOS HILOS.
	SOLUCION: SEPARAR CREACION Y JOIN EN DOS BUCLES.

*/
int var = 0;
pthread_mutex_t mutex; // estructura de datos para sincronizar procesos

/*con un numero grande de operaciones en paralelo, empiezan a surgir imprecisiones*/
void* routine(){

	for (int i = 0; i < 10000000; i++)
	{
		//un hilo a la vez realizara la operacion
		pthread_mutex_lock(&mutex); 
		var++; 
		pthread_mutex_unlock(&mutex);

	}

}


int main(int argc, char const *argv[])
{
	pthread_t th[4]; //cuatro hilos en el proceso
	int error;

	pthread_mutex_init(&mutex, NULL); // inicializar la referencia a mutex

	//SEPARAR CREATE Y JOIN EN DOS BUCLES PARA CONSERVAR EL PARALELISMO DE LOS HILOS

	//paso como argumento el hilo, los atributos de hilo, la funcion y atributos de la funcion (?)	
	for (int i = 0; i < 4; i++)
	{

	    // En pthread_create se manda la direccion. pthread_create(&hilo, NULL, &routine, NULL)
		if ( pthread_create(th+i, NULL, &routine, NULL)!= 0 )
		{
			perror("pthread_create: ");
			return 1;
		}

		printf("Hilo %d creado\n", i);

		

	}


	//Sin join no se ejecuta nada...
	for (int i = 0; i < 4; i++)
	{
		//En pthread_join se manda el "valor". pthread_join(hilo, NULL)
		if (pthread_join(th[i], NULL) !=0)
		{
		 	perror("pthread_join: ");
		 	return 2;
		}

		printf("Hilo %d iniciado\n",i);

	}

	pthread_mutex_destroy(&mutex);
	printf("Valor de var:%d\n",var);

	return 0;
}