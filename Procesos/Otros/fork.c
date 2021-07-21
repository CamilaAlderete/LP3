#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*DUDA: AL CREAR UN PROCESO HIJO, PARECE QUE AMBOS PROCESOS PADRE E HIJO
EJECUTAN EL MISMO CODIGO QUE ES ESCRITO LUEGO DE DE LLAMAR A FORK

EL PROCESO HIJO SABE DE LA EXISTENCIA DE LA VARIABLE i (QUE FUE DECLARADO
EN EL PADRE ANTES DE USAR fork() ), PERO NO SABE DE NINGUN CAMBIO QUE EL PADRE
REALIZA A i LUEGO DE fork().


*/

int main(){

	int i=1;

	int pid =fork();

	if (pid<0)
	{
		fprintf(stderr, "Fork did not succeed");
		exit(-1);
	}else if(pid>0){ 
		//padre
		i++;
		printf("Process-ID of the child is %i \n", pid);
		printf("For the father, i is %i\n",i);	
	}else{
		//pid == 0 es un hijo
		printf("I am the child, my process-ID is %i \n", getpid());
		printf("For me, i still is %i\n",i);
	}


	return 0;

}