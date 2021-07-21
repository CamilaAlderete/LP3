#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define MAX 20
int array[MAX] ={0};
char* emoji_array[MAX];
pthread_mutex_t mutex;

int indice = -1;

typedef struct
{
    int ini;
    int fin;
} datos;

bool exit_ = false;
bool encontrado=false;
int hilos_finalizados=0;
int nro_hilos;

int key;

char *lupa="\360\237\224\215\357\270\217\0";
char *vacio="  \0";
char *foco="\360\237\222\241\0";

void generarArray(){
    
    srand(time(0));
    int numero;    
    
    for (int i = 0; i < MAX; i++)
    {
        numero = rand()%10;
        
        //que nunca aparezca el nro buscado.
        // while (numero==4)
        // {
        //     numero = rand()%10;
        // }
        
        
        array[i]=numero;
    }

    for (int i = 0; i < MAX; i++)
    {
        emoji_array[i]=vacio;
    }

    
}

void printArray2(){
    
    // for (int i = 0; i < MAX; i++)
    // {
    //    printf("%d|",i);
    // }
    // printf("\n");

    /*for (int i = 0; i < MAX; i++)
    {
       printf(" %d ",i);
    }
    printf("\n");*/


    for (int i = 0; i < MAX; i++)
    {
       printf(" %d ",array[i]);
    }
    printf("\n");

    for (int i = 0; i < MAX; i++)
    {
       printf(" %s",emoji_array[i]);
    }
    printf("\n");
}

void printArray1(int ini, int fin){
    
    printf(" [");
    for (int i = ini; i <=fin; i++)
    {
       printf("%d",array[i]);
       if (i!=fin)
       {
           printf(", ");
       }
       
    }
    printf("] ");
}

void* busqueda(void *arg){
    datos *dato = (datos*)malloc(sizeof(datos*));
    dato = (datos*) arg;

    int ini = dato->ini;
    int fin = dato->fin;
    int i;
    
    emoji_array[ini]=lupa;//por si no logra ingresar al bucle
    
    //for ( i = ini; i <= fin && exit_==false && encontrado==false; i++)
    for ( i = ini; i <= fin && encontrado==false; i++)
    {
        if (array[i]!=key)
        {
            emoji_array[i]=lupa;
            
        }else //si es el valor que se busca
        {
            emoji_array[i]=foco;
            encontrado=true;

            pthread_mutex_lock(&mutex);
                indice = i;
                hilos_finalizados++;
		    pthread_mutex_unlock(&mutex);
            
            pthread_exit(NULL);
        }

        sleep(1);
        if (encontrado==false)
        {
            emoji_array[i]=vacio;
        }
            
    }

    pthread_mutex_lock(&mutex);
        hilos_finalizados++;
	pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
    
    

}

void particion(int salto, int nro_hilos){

    //particionado y creacion de hilos
    int j=0;
    for (int i = 1; i < nro_hilos; i++)
    {
        //printArray1(j,j-1+salto);
        
        //datos del rango de busqueda del hilo
        datos *dato;
        dato = (datos*)malloc(sizeof(datos*));
        dato->ini = j;
        dato->fin = j-1+salto;

        //hilo
        pthread_t *t;
        t = (pthread_t *) malloc(sizeof(pthread_t));
        pthread_create(t, NULL, &busqueda,dato);
        
        j=j+salto;
    }

    // printArray1(j,MAX-1);

    //datos del rango de busqueda del hilo
    datos *dato;
    dato = (datos*)malloc(sizeof(datos*));
    dato->ini = j;
    dato->fin = MAX-1;

    //hilo
    pthread_t *t;
    t = (pthread_t *) malloc(sizeof(pthread_t));
    pthread_create(t, NULL, &busqueda,dato);
    //printf("\n");

}

int calcularParticion(int nro_hilos){

    int resultado;

    //calculo de las particiones
    if (MAX%2==0)
    {
        if (nro_hilos%2==0)
        {
            resultado = (int)ceil(MAX/nro_hilos);
        }else
        {
            resultado = (int)floor(MAX/nro_hilos);
        }
        
        
    }else
    {
        if (nro_hilos%2==0)
        {
            resultado = (int)floor(MAX/nro_hilos);
        }else
        {
            resultado = (int)ceil(MAX/nro_hilos);
        }
    }

    return resultado;   

}

void* vista(void *arg){
    while (encontrado==false && hilos_finalizados!=nro_hilos)
    {
        system("clear");
        printArray2();
        //printf("\nFinalizado: %d\n", hilos_finalizados);
        sleep(1);
    }

    system("clear");
    printArray2();
    if (encontrado==true)
    {
        printf("Encontrado en: %d\n",indice);
    }else{
        printf("No encontrado\n");
    }
    
    //printf("\nFinalizado: %d\n", hilos_finalizados);



}

int main(int argc, char const *argv[])
{
    system("clear");
    if (argc!=3)
    {
      printf("Se necesitan 3 argumentos\n");
      return -1;
    }

    //obtencion de los datos de la terminal
    key = atoi(argv[1]);
    nro_hilos = atoi(argv[2]);

    if (nro_hilos>MAX)
    {
        printf("Nro. de hilos mayor a tamano del arreglo.");
        return -1;
    }
    
    pthread_mutex_init(&mutex, NULL); 

    //generacion de array aleatorio
    generarArray();
    //printArray2();

    //calculo de las particiones
    int salto = calcularParticion(nro_hilos);
    
    //creacion de hilos y ejecucion de la busqueda
    particion(salto,nro_hilos);

    //vista 
    pthread_t *v;
    v = (pthread_t *) malloc(sizeof(pthread_t));
    pthread_create(v, NULL, &vista,NULL);
    pthread_join(*v,NULL);
    
    pthread_mutex_destroy(&mutex);

    return 0;
}
