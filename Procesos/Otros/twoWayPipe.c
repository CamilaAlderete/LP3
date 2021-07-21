#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>  // waitpid()


/*

	1-Padre envia una variable al hijo
	2- Hijo realiza una operacion con la variable
	3- El nuevo valor retorna al padre

	5 => 5*4 => 20

	Obs.: Un pipe es unidireccional, si en un extremo se escribe, 
	en el otro se debe leer y viceversa.
	Si se desea una comunicacion en dos direcciones, se debe emplear dos
	pipes, un pipe con sentido distinto respecto al otro.

*/

int main(int argc, char const *argv[])
{
	int p1[2]; //hijo a padre
	int p2[2]; // padre a hijo

	if(pipe(p1) == -1){ return 1;}
	if(pipe(p2) == -1){ return 2;}

	int pid = fork();

	if (pid ==0)//hijo
	{
		int x;
		close(p1[0]);
		close(p2[1]);
		
		if (read(p2[0],&x, sizeof(x)) == -1){return 3;}
		printf("Recevied %d\n",x);
		x*=4;

		if (write(p1[1], &x,sizeof(x)) == -1){return 4;}
		printf("Wrote 5*4 =%d\n",x);


		close(p1[1]);
		close(p2[0]);

	}else if( pid>0){ //padre

		close(p1[1]);
		close(p2[0]);

		int y= 5;
		if(write(p2[1], &y, sizeof(y)) == -1){ return 5;}

		if(read( p1[0], &y, sizeof(y)) == -1){ return 6;}
		
		printf("Result: %d\n",y);
		close(p1[0]);
		close(p2[1]);
		wait(NULL); 

	}






	return 0;
}