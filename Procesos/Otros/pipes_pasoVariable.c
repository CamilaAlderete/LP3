#include <stdio.h>     // printf(), perror(), sprintf()
#include <unistd.h>    // fork(), pid_t, getpid(), getppid(), pipe(), open(), close()


/* Comunicacion entre procesos
	Al crear el pipe, cada proceso tiene los extremos (file descriptor):
		fd[0] - read
		fd[1] - write

	Ejemplo:Se asignara un valor a una variable en el hijo
	y se leera en el padre
*/

int main(int argc, char const *argv[])
{
	  
	int fd[2]; //file descriptor

	if (pipe(fd) == -1) //se abre pipe
	{
		printf("Error al abrir el pipe\n");
		return 1;
	}

	int id = fork();
	
	if (id==0) //proceso hijo
	{		
		close(fd[0]); // se cierra porque no se leera nada

		int varHijo;
		printf("Proceso hijo - Ingrese un numero entero: ");
		scanf("%d", &varHijo);

		write(fd[1], &varHijo, sizeof(int)); // se escribe (para compartir)
		close(fd[1]);

	}else{ //proceso padre

		close(fd[1]); // se cierra extremo de escritura, porque no se va escribir nada

		int varPadre;
		read(fd[0], &varPadre, sizeof(int));//se lee la variable compartida por el hijo
		close(fd[0]); // se cierra extremo de lectura

		printf("Proceso padre - El valor asignado a la variable es: %d\n",varPadre);

	}

	return 0;
}