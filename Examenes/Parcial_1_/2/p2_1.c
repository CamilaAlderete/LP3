
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


// Terminal: kill -l


static const char* name;

char const* signal_to_str(int nro_signal) {
    switch(nro_signal) {
	    #include "signal_to_str_cases.inc"
	    default: return "<unknown>";
	}
}


int sendSignal(pid_t pid,int nro_signal,time_t delay){
	
	
	sleep(delay);
	const char* name = signal_to_str(nro_signal);
	printf("\n\nPID:%d SIGNAL:%d DELAY:%ld\n",pid,nro_signal,delay);
	printf("NAME:%s\n",name);

	//enviar senal	
	if(nro_signal>0 && nro_signal<SIGRTMAX){ //si esta dentro del rango
		if ( kill(pid, nro_signal) == -1 )
		{
			printf("El proceso con pid:%d no existe\n",pid);
			return -1;

		}
	
	}else{
		printf("Senal no valida\n");
		return -1;
	}

	printf("Senal nro. %d fue enviada\n", nro_signal);
	printf("\n");

	return 0;

}

void lectura(FILE *fp){

	int n = 15;
	char str_pid[n];
	char str_signal[n];
	char str_delay[n];
	pid_t _pid;
	pid_t pid;
	time_t delay;
	int nro_signal;


	// LEE PALABRA POR PALABRA
	fscanf(fp,"%s",str_pid);//ProcesoID
	fscanf(fp,"%s",str_signal);//Señal
	fscanf(fp,"%s",str_delay);//SegundosDelay

	while( fscanf(fp,"%s",str_pid) == 1 ){

		fscanf(fp,"%s",str_signal);
		fscanf(fp,"%s",str_delay);

		pid = atoi(str_pid);
		nro_signal = atoi(str_signal);
		delay = atoi(str_delay);

		
		_pid = fork();
		if (_pid < 0)
		{
			perror("fork");
			exit(-1);

		}

		if (_pid ==0)//PROCESO HIJO
		{

			sendSignal(pid, nro_signal,delay);
			exit(0);	
		}

	}


	return;
}

int main(int argc, char const *argv[])
{


	//falta handler...
	FILE *fp;
	
	fp = fopen("test.txt", "r");
	
	if(fp == NULL){
		perror("fopen:");
		exit(-1);
	}

	lectura(fp);	

	fclose(fp);

	return 0;
}


/*	LEE POR LINEAS

	while ( fgets(cadena, sizeof(cadena), fp) != NULL){		
        printf("%s ",cadena);
		fputs(cadena, stdout); //escribe
			
	}
*/

/*  LEE POR PALABRAS... PROBLEMA: SE IMPRIME DOS VECES LA
	ULTIMA CADENA DEL ARCHIVO PORQUE fscanf NUNCA SE VUELVE NULL  O 
	ALGO POR EL ESTILO.
	
	while( !feof(fp) ){
	 	fscanf(fp,"%s",cadena);
		printf("%s\n",cadena);
	}

*/
/*	if(nro_signal != SIGKILL && nro_signal!=SIGSTOP ){
			printf("La senal %s fue enviada\n", name);
			printf("\n");
			return 0;

		}*/