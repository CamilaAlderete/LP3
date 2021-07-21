#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*a este proceso se lo debera "matar" 
colocando su pid y el nro 9 en el archivo de texto 
*/
int main(int argc, char const *argv[])
{

	pid_t pid;
	pid = getpid();
	printf("%d\n",pid);

	int pid1 =fork();

	printf("%d\n",pid1);

	while(1){
		//nunca termina 
	}
	return 0;
}