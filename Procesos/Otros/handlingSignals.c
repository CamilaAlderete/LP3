#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/*
	En el ejemplo se evitara que el proceso pase a 
	segundo plano con CTRL + Z
*/


void handle_sigtstp(int sig){
	printf("Stop not allowed\n");
}

int main(int argc, char const *argv[])
{
	
	//ejemplo 1, se usa estructura para portabilidad
	struct sigaction sa;
	sa.sa_handler = &handle_sigtstp;
	sa.sa_flags = SA_RESTART;

	sigaction(SIGTSTP, &sa, NULL);

	// signal(SIGTSTP, &handle_sigtstp); //simplificado pero puede no ser portable

	int x;
	printf("Input number: ");
	scanf("%d", &x);
	printf("Result %d * 5= %d\n",x , x*5);


	return 0;
}