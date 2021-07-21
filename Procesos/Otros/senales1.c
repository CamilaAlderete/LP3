#include <stdio.h>     // printf(), perror(), sprintf()
#include <unistd.h>    // fork(), pid_t, getpid(), getppid(), pipe(), open(), close()
#include <time.h>
#include <sys/wait.h>
#include <signal.h>

/*
	Las senales sirven para controlar los procesos
	kill(): no necesariamente significa "matar" el proceso,
	puede ingresarse otra instruccion

*/

int main(int argc, char const *argv[])
{
	int pid = fork();

	if (pid== -1) // error al crear proceso hijo
	{
		return 1;
	}

	if (pid==0) //proceso hijo
	{
		while(1){
			printf("Texto\n");
			usleep(50000);//suspende la ejecucion unos segundos
		}

	}else{ //proceso padre
		sleep(1); //esperar un segundo antes de enviar senal
		kill(pid, SIGKILL);//envia senal al hijo con esew pid para "matarlo"  
		wait(NULL); //espera a la terminacion del hijo

	}
	return 0;
}
