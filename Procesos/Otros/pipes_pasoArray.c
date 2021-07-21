#include <stdio.h>     // printf(), perror(), sprintf()
#include <unistd.h>    // fork(), pid_t, getpid(), getppid(), pipe(), open(), close()
#include <stdlib.h>	   //malloc()

/* Comunicacion entre procesos
	Al crear el pipe, cada proceso tiene los extremos (file descriptor):
		fd[0] - read
		fd[1] - write
	
	Ejemplo: el proceso padre leera el array del proceso hijo por medio de pipes

*/



int main(int argc, char const *argv[])
{

	int fd[2];//file descriptor

	if (pipe(fd) == -1)
	{
		return 2;
	}


	int pid = fork();
	if (pid == -1) //error al crear proceso hijo
	{
		return 3;		
	}


	if (pid == 0) //proceso hijo
	{
		int array[10]={1,2,3,4,5,6,7,8,9,10};
		int n = sizeof(array)/sizeof(array[0]);

		// se cierra el extremo de lectura porque no se usara en el hijo
		close(fd[0]);

		
		//procedimiento de escritura en el proceso hijo
		if ( write(fd[1], &n, sizeof(int)) < 0) //tamanio del array
		{
			return 4;
		}

		if ( write(fd[1], array, sizeof(int) * n ) < 0) //array
		{
			return 5;
		}

		close(fd[1]);// se cierra el extremo escritura


	}else{ //proceso padre

		int n;

		close(fd[1]); //cierra su extremo de escritura
		
		//procedimiento de lectura		
		if ( read(fd[0], &n, sizeof(int)) < 0) //tamanio arreglo
		{
			return 6;	
		}

		// definicion de arreglo con tamanio n
		int* array;
		array = (int*) malloc(n * sizeof(int));


		if ( read(fd[0], array, sizeof(int) * n) <0) //lectura del arreglo
		{
			return 7;
		}

		close(fd[0]);// se cierra extremo de lectura

		for (int i = 0; i < n; ++i)
		{
			printf("%d ",array[i]);
		}		

	}
	return 0;

}