#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
/*
	Se crea un hilo que ejecuta una funcion. Esta funcion retorna un nro
	aleatorio entre 1 a 6 (como un dado). Por medio de join se recupera 
	ese valor y se imprime en pantalla.

	Recordatorio:
	- int* ptr : puntero del tipo int
	- ptr = Otro_ptr : se asigna a ptr la direccion de Otro_ptr
	- *ptr = 100 : se asigna un valor a un puntero con cierta direccion;
*/

void* roll_dice()
{
	int valor = (rand() % 6)+1;

	/*se asigna espacio a un puntero "resultado" ya que no se 
	puede retornar "valor" porque es una variable local de la funcion. 
	*/  
	int* resultado = malloc(sizeof(int));
	*resultado = valor;

	return (void*) resultado;

}


int main(int argc, char const *argv[])
{
	pthread_t p;
	int* resultado;

	srand(time(NULL)); // con cada ejecucion, generara nros aleatorios

	if ( pthread_create(&p, NULL, &roll_dice, NULL) != 0 )
	{
		perror("pthread_create: ");
		return 1;
	}

	if ( pthread_join(p, (void**) &resultado) != 0)
	{
		return 2;
	}

	printf("Resultado: %d\n", *resultado);

	free(resultado);
	/*libera espaci reservado a resultado en la funcion roll_dice()
	  En programas extensos hay que tener cuidado y no se deberia hacer esto
	  la reserva y liberacion de memoria deberia hacerse en un mismo "lugar"*/
	return 0;
}