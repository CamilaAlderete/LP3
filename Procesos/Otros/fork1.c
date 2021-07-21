/* 
	https://youtu.be/VwjP-KFuZCM?list=PLn9-gi1mj5U7K_Ke9N3uqOFdzNnHUpHhl

	**fork(). PROCESOS PADRE, HIJO, HUERFANO Y ZOMBIE

	pid_t fork(void)
		- CREA UN NUEVO PROCESO ("HIJO")
		- EL PROCESO HIJO ES UN DUPLICADO DEL PADRE
		- LOS DOS PROCESOS:
			-TIENEN PIDS DIFERENTES
			- CORREN EN ESPACIOS DE MEMORIA SEPARADOS

	RETORNO DE fork()
		>SI TODO VA BIEN: 
			-RETORNO AL PADRE: PID DEL HIJO
			- RETORNO DEL HIJO:0

		>SI HAY ERRORES:
			-RETORNO AL PADRE: -1
			(EL HIJO NO ES CREADO)

*/

/*
	EL ESPACIO DE MEMORIA DEL PROCESO HIJO ES UNA COPIA DEL LA MEMORIA DEL PROCESO PADRE
	ENTONCES AMBOS TIENEN LOS MISMOS VALORES, A PARTIR DE AHI CADA PROCESO PUEDE CAMBIAR
	LAS VARIABLES COMO QUIERA Y LO QUE HAGA UNO NO INFLUIRA EN EL OTRO
*/

#include <unistd.h>
#include <stdio.h>

int var = 22;

int main(void){

	pid_t pidC;

	printf("**proceso PID=%d comienza\n",getpid() ); //padre
	pidC = fork();

	printf("proceso PID=%d, pidC =%d ejecutandose\n", getpid(), pidC); //hijo

	if(pidC>0){
		//se esta ejecutando el padre
		var =44;

	}else if( pidC==0){
		//ejecutandose el proceso hijo
		var=33;

	}else{
		//error -1 (se notifica la padre el error)
	}

	//suponiendo que ambos proceos se ejecutan en un ciclo sin fin...
	while(1){ //esto sera ejecutado tanto por el padre como por el hijo
		sleep(2);
		printf("proceso PID=%d, var =%d ejecutandose\n", getpid(), var);
	}
	return 0;
}

/*

	EN TERMINAL SE COMPILA EL ARCHIVO: gcc fork.c -o fork
	SE EJECUTA fork
	SE VISUALIZA COMO LOS PORCESOS PADRE E HIJO "CORREN" INDEFINIDAMENTE

	EN OTRA TERMINAL...
	SE EJECUTA EL COMANDO ps lf DONDE SE VISUALIZA LOS PROCESOS EJECUTANDOSE
	
	1) SE ELIMINA EL PROCESO PADRE:...
		kill -9 [pid_padre]
		EL PORCESO HIJO CONTINUA EJECUTANDOSE
		ps lf 
		SE PUEDE VISUALIZAR QUE EL PROCESO HIJO HA SIDO ADOPTADO POR OTRO PROCESO
		ps lp [pid_padreAdoptivo], SE VE COMO SE LLAMA EL NUEVO PROCESO PADRE

	2) SE ELIMINA EL PORCESO HIJO:...
		kill -9 [pid_hijo]
		ps lf
		EL PORCESO PADRE SIGUE EJECUTANDOSE Y EL HIJO SE CONVIERTE EN ZOMBIE
		ps lf
		EL ZOMBIE (STATUS Z+) SIGUE REPRESENTADO EN EL SISTEMA Y OCUPA ALGUNOS RECURSOS


*/