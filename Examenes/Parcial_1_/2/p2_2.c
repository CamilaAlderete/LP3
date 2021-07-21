#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>


static const char* name;

char const* signal_to_str(int nro_signal) {
    switch(nro_signal) {
	    #include "signal_to_str_cases.inc"
	    default: return "<unknown>";
	}
}

void handler(int nro_signal, siginfo_t *info, void *ptr)
{
	name = signal_to_str(nro_signal);
    // write(STDERR_FILENO, name, sizeof(name)); // no es seguro usar printf... no se porque
    printf("%s",name);

}

void conf_handler(int nro_signal)
{
	if(nro_signal != SIGKILL && nro_signal!=SIGSTOP ){

	    static struct sigaction sa;

	    memset(&sa, 0, sizeof(sa));
	    sa.sa_sigaction = handler;
	    sa.sa_flags = SA_RESTART;

	    sigaction(nro_signal, &sa, NULL);
	}
}


/**Se generan n hijos, que envian envian senales aleatorias**/
int main(int argc, char const *argv[])
{
	int senal_aleatoria;
	srand(time(0));
	pid_t pid;
	int n= 10;
	
	for(int i=0;i<n;i++) 
    { 

    	sleep(1);
    	//se genera senal aleatoria
    	int senal_aleatoria = rand()%SIGRTMAX +1;
        
        if((pid =fork()) == 0) //hijo
        { 
        	//se configura handler
        	conf_handler(senal_aleatoria);

        	printf("PID:%d SIGNAL:%d NAME:", getpid(),senal_aleatoria);

        	//se envia senal a si mismo?? 
			if ( kill(getpid(), senal_aleatoria) == -1 ){
				perror("kill");
			}

			printf("\n\n");

            exit(0); //termina proceso hijo

        }else{
        	int estado;
       		 waitpid(pid,&estado,0); 	
        } 
   
    } 


	return 0;
}