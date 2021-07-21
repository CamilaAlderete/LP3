/*

	PROBLEMA 1:

	Se quiere realizar un programa que cree un conjunto de procesos que acceden en exclusión mutua a un fichero compartido por todos ellos. Para ello, se deben seguir los siguientes pasos:
	a) Escribir un programa que cree N procesos hijos. Estos procesos deben formar un anillo como el que se
	muestra en la figura 1.1. Cada proceso en el anillo se enlaza de forma unidireccional con su antecesor y su
	sucesor mediante un pipe. Los procesos no deben redirigir su entrada y salida estándar. El valor de N se recibirá como argumento en la línea de mandatos. Este programa debe crear además, el fichero a compartir
	por todos los procesos y que se denomina anillo.txt

	b) El proceso que crea el anillo inserta en el mismo un único carácter que hará de testigo, escribiendo en el
	pipe de entrada al proceso 1. Este testigo recorrerá el anillo indefinidamente de la siguiente forma: cada
	proceso en el anillo espera la recepción del testigo; cuando un proceso recibe el testigo lo conserva durante
	5 segundos; una vez transcurridos estos 5 segundos lo envía al siguiente proceso en el anillo. Codifique la
	función que realiza la tarea anteriormente descrita. El prototipo de esta función es:
	void tratar_testigo(int ent, int sal);
	donde ent es el descriptor de lectura del pipe y sal el descriptor de escritura



	c) Escribir una función que lea de la entrada estándar un carácter y escriba ese carácter en un fichero cuyo
	descriptor se pasa como argumento a la misma. Una vez escrito en el fichero el carácter leído, la función escribirá por la salida estándar el identificador del proceso que ejecuta la función.

	d) Cada proceso del anillo crea dos procesos ligeros que ejecutan indefinidamente los códigos de las funciones
	desarrolladas en los apartados b y c respectivamente. Para asegurar que los procesos escriben en el fichero
	en exclusión mutua se utilizará el paso del testigo por el anillo. Para que el proceso pueda escribir en el fichero debe estar en posesión del testigo. Si el proceso no tiene el testigo esperará a que le llegue éste. Nóte -
	se que el testigo se ha de conservar en el proceso mientras dure la escritura al fichero. Modificar las
	funciones desarrolladas en los apartados b y c para que se sincronicen correctamente utilizando semáforos.

*/


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

	int fd[2];
	int entrada;
	int salida;
	pid_t pid;
	int i;
	int N;
	char c;


	if (argc!=2)
	{
		printf("Error al ingresar argumento N...\n");
		return 0;
	}

	N = atoi(argv[1]);

	// se crea pipe del proceso
	if(pipe(fd) < 0){
		perror("Error al crear pipe\n");
		return 0;
	}

	entrada = fd[0];
	salida = fd[1];
	write(salida,&c,1); //se escribe  testigo en el primer pipe

	for(i=0; i<N; i++){

		//si no es el ultimo proceso a crear
		if(i!= N-1){

			if(pipe(fd) < 0){
				perror("Error al crear pipe\n");
				return 0;
			}

			pid = fork(); 
			/*el padre y los hijos saben del for de arriba, por lo que mas adelante al proceso hijo
			se le coloca break para salir del ciclo*/
			
			if(pid==0){
				printf("Proceso %d\n",i);
			}

			switch(pid){
				case -1: /*error*/
					perror("Error en el fork");
					return 0;
				case 0: /*hijo*/
					//no es el ultimo en ser creado
					if(i != N-1) 
					{
						close(salida);
						salida = dup(fd[1]); // Duplicación de un descriptor:  int dup (df_antiguo)...busca el descriptor de fichero más pequeño que exista en la tabla de descriptores de ficheros ylo referencia hacia el mismo fichero que df_antiguo
						close(fd[0]);
						close(fd[1]);
					}

					i=N;
					break;
				default: //padre
					if(i== N-1) //ultimo proceso
						return 0;
					else{
						close(entrada);
						close(fd[1]);
						close(fd[0]);
					}

					break;
			}
			if(pid==0){
				printf("Proceso %d fuera de switch\n",i);
			}
		}

		if(pid==0){
				printf("Proceso %d fuera del if\n",i);
		}
	}

	if(pid==0){
				printf("Proceso %d fuera del for\n",i);
	}

	/*a continuacion los procesos anillo continuaran sus acciones*/
	return 0;
}

/* TERMINOS

	DESCRIPTOR DE ARCHIVOS:En los sistemas Unix, los descriptores de archivo se pueden referir a archivos, 
	directorios, dispositivos de bloques o dispositivos de caracteres (también llamados "archivos especiales"),
	sockets, FIFOs (también llamados "tuberías con nombre") o tuberías sin nombre.
	https://es.wikipedia.org/wiki/Descriptor_de_archivo

*/