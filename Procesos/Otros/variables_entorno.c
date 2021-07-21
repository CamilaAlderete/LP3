#include <stdio.h>

/*

	argc: numero de argumentos
	argv: "valores o nombres de los argumentos"
	envp: variables de entorno

*/

int main(int argc, char const *argv[], char *envp[])
{

	printf("I was called width %i arguments\n", argc);

	printf("My arguments are: \n");

	while(*argv != 0){
		printf("%s \n", *argv++);
	}

	printf("My environment-variables are:\n");
	while(*envp != 0){
		printf("%s \n", *envp++);
	}
	
	return 0;
}