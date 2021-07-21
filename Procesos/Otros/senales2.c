#include <stdio.h>     // printf(), perror(), sprintf()
#include <unistd.h>    // fork(), pid_t, getpid(), getppid(), pipe(), open(), close()
// #include <time.h>
#include <sys/wait.h> 	//wait()
#include <signal.h>		//kill()

/*
	Las senales sirven para controlar los procesos
	kill(): no necesariamente significa "matar" el proceso,
	puede ingresarse otras instrucciones

	En el ejemplo, desde el proceso padre se enviara senales al hijo.
	El proceso hijo debera ejecutarse un nro. determinado de segundos y luego pausarse.
	Al ingresar un numero negativo, el padre debera terminar al proceso hijo.

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

		kill(pid, SIGSTOP);//proceso hijo se pausa

		int t;
		do{
			printf("Time in seconds for execution:");
			scanf("%d", &t);

			if (t > 0)
			{
				//el proceso hijo se ejecutara un determidado intervalo de segundos
				kill(pid, SIGCONT);//continua
				sleep(t);
				kill(pid, SIGSTOP);//pausa
				
			}

		}while(t>0);


		kill(pid, SIGKILL);
		wait(NULL);// no se que funcion cumple...
	}

	return 0;
}