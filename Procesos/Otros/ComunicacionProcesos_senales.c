#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
/*
	En el proceso padre se ingresara un valor y sobre
	este se realizara una operacion.
	El proceso hijo lanzara una senal al padre en caso
	de que se cumpla cierto tiempo sin ingresar el dato
	requerido.
*/

int x=0; //variable global para detectar cambio

void handle_sigusr1(int sig){
	if (x==0)
	{
		printf("\nRecuerda qe la multiplicacion es suma repetitiva\n");
		
	}
}


int main(int argc, char const *argv[])
{
	
	int pid = fork();

	if(pid == -1){
		return 1;
	}

	if (pid == 0)//proceso hijo
	{
		sleep(5);//5 segundos antes de desplegar mensaje
		kill( getppid(), SIGUSR1);//el proceso hijo despliega la senal al padre

	
	}else{ //proceso padre

		struct sigaction sa = {0};
		sa.sa_flags = SA_RESTART;
		sa.sa_handler = &handle_sigusr1;
		sigaction(SIGUSR1, &sa, NULL);

		printf("Cual es el resultado de 3 x 5: ");
		scanf("%d", &x);

		if (x==15)
		{
			printf("Correcto!");
		}else{
			printf("Mal...");
		}

	}



	return 0;
}