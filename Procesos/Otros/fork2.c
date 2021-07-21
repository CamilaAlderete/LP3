/*

	**FORK, WAIT Y MULTIPLES PROCESOS HIJOS** 

	pid_t fork(void);

	pid_t wait( int *wstatus); ...ESPERAR QUE UN PORCESO HIJO TERMINE

	WEXITSTATUS(wstatus) .... LA VARIABLE wstatus SE INSPECCIONA CON UNA SERIE DE MACROS 
	EL CUAL DARA EL VALOR DE TERMINACION DE UN HIJO 

*/

#include <unistd.h> // fork
#include <sys/wait.h>//wait 
#include <stdio.h>
#include <stdlib.h>//valores aleatorios

#define NUM_CHILD 5

//se creara un nro aleatorio
int doSomething(void){
	int ret;

	srand( getpid() ); //semilla
	ret = (rand() % 256); // de cero a 255
	printf("HIJO: PDI=%d, valor aleatorio calculado %d \n",getpid(),ret);

	return ret;
}

int main(void){

	pid_t pidC;
	int status;

	//se crearan hijos
	for (int i = 0; i < NUM_CHILD; i++)
	{
		pidC = fork(); // se crea hijo

		if (pidC >0) 
		{
			//si el padre se esta ejecutando, queremos que siga haciendo hijos
			continue;
			
		}else if( pidC ==0){ //hijo
			//no quiero que el hijo haga hijos
			// el valor de terminacion sera un nro aleatorio
			//una vez que el hijo ejecuta esa funcion, acaba como proceso
			exit(doSomething());// el proceso tendra un numero de terminacion que fue generado por la funcion

		}else{
			//error
		}
	}


	//esperar a que los hijos terminen y ver que valores han retornado
	for (int i = 0; i < NUM_CHILD; i++){ // lo ejectara el padre 5 veces

		pidC =wait(&status);
		printf("PADRE de PID = %d, hijo de PID = %d terminado, st = %d \n", getpid(), pidC, WEXITSTATUS(status));

	}


	//el padre quedara en un bucle infinito
	while(1){
		sleep(10); //se dormira para siempre
	}

	return 0;
}