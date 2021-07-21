
#include <stdio.h>     // printf(), perror(), sprintf()
#include <stdlib.h>    // atoi(), exit(), EXIT_FAILURE

#include <string.h>    // strcpy(), memset(), 

#include <unistd.h>    // fork(), pid_t, getpid(), getppid()

#include <sys/wait.h>  // waitpid()

#include <sys/ipc.h> 
#include <sys/shm.h>

// FILE *fp;
int total;
int size;
void printArray(int A[], int i, int j);
void cargar_array(int *array, int n);
// void printNodo(int A[], int i, int j);
// void printArbol(int arr[], int izq, int der, int *proceso);




void asignar(int **dato_compartido, int tamanio, int* id_dato_compartido){
        
    key_t key = IPC_PRIVATE; 

    size_t size = sizeof(int)*tamanio; 

    // asigna un segmento de memoria compartida
    if (( *id_dato_compartido = shmget(key, size, IPC_CREAT | 0666)) < 0) 
    { 
        perror("shmget"); 
        _exit(1); 
    } 


    //el segmento existe en algun lugar, falta "asociar o vincular" al espacio de memoria del proceso    
    if (( *dato_compartido = shmat( *id_dato_compartido, NULL, 0)) == (int *) -1) 
    { 
        perror("shmat"); 
        _exit(1); 
    }

    return;

}




void eliminar(int **dato_compartido, int id_dato_compartido){
        
       
        if ( shmdt(*dato_compartido) == -1) 
        { 
            perror("shmdt1"); 
            _exit(1); 
        } 

        if (shmctl( id_dato_compartido, IPC_RMID, NULL) == -1) 
        { 
            perror("shmctl"); 
            _exit(1); 
        }

        return;
}



void merge(int arr[], int izq, int m, int der)
{
    int i, j, k;
    int n1 = m - izq + 1;
    int n2 = der - m;
 
    int L[n1], R[n2];
 
    for (i = 0; i < n1; i++)
        L[i] = arr[izq + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    

    i = 0;
    j = 0; 
    k = izq; 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
   
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }




}

void MergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        MergeSort(arr, l, m);
        MergeSort(arr, m + 1, r);
 
        merge(arr, l, m, r);
    }
}



void mergeSort(int arr[], int izq, int der, int *proceso)
{
    if ( der>izq) {

        //guardar nro de proceso antes que otro proceso paralelo lo cambie
        int n = *proceso;

        // impresion de sub-array
        printf("Proceso %d: ",n);
        printArray(arr, izq, der+1);
        printf("\n\n");

        int m = izq + (der - izq) / 2;
 

        int pid_Izq, pid_Der;
        pid_Izq = fork();//primer hijo

        if (pid_Izq!=0)// si no es proceso hijo, creo otro hijo
        {
            pid_Der = fork();
        }

        //verificar que ambos procesos se hayan creado
        if (pid_Izq == -1 && pid_Der == -1)
        {
            perror("Error al crear procesos hijos\n");
            return;
        }

        //los procesos hijos ordenaran una mitad cada uno
        if (pid_Izq == 0) //HIJO IZQUIERDO
        {   
            usleep(1);
            if(izq<m){

                if (*proceso< total-2)
                {
                                
                    *proceso+=1;

                    mergeSort(arr, izq, m, proceso);

                }else{
                  /*en caso de que se llego al limite de procesos y 
                  el arreglo todavia no este ordenado*/  
                  MergeSort(arr, izq, m);
                }
            }
            _exit(0); 

        }else if(pid_Der == 0) //HIJO DERECHO
        {
            usleep(2);

            if (m<der)
            {
                if (*proceso< total-1)
                {
                    *proceso+=1;

                    mergeSort(arr,m+1,der, proceso);

                }else{
                    /*en caso de que se llego al limite de procesos y 
                  el arreglo todavia no este ordenado*/
                  MergeSort(arr, m+1,der);

                }
            }

            _exit(0); 
       
        }else{ //PADRE

            //esperar a la terminacion de los hijos
            int estado;
            waitpid(pid_Izq, &estado, 0); 
            waitpid(pid_Der, &estado, 0);
            
            merge(arr, izq, m, der); //juntar sub-arrays ordenados parcialmente

            // imprimir resultado parcial
            printf("Proceso %d: ",n);
            printf("{");
            printArray(arr, izq, der+1);
            printf("} = {");
            printArray(arr,izq,m+1);
            printf("} y {");
            printArray(arr, m+1, der+1);
            printf("}");
            printf("\n");
            printf("\n");
            
        }
        
    }
}




int contar(char* str, const char s[]){
   
   int size=0;
   char *token;
   token = strtok(str, s);

   // recorre la cadena y toma los datos de acuerdo al delimitador "," en s
   while( token != NULL ) {
      // printf( " %s\n", token );
      size++; //contar cantidad de "tokens"
      token = strtok(NULL, s);
      
   }

   return size;
}

void str_to_int(char str[] ,const char s[],int array[]){

   int i=0;
   char *token;
   token = strtok(str, s);
   

   // toma datos de str, convierte a int y guarda en array
   while( token != NULL ) {

      array[i]= atoi(token);
      i++;
      token = strtok(NULL, s);

   }

   return;   
}



int main(int argc, char const *argv[])
{


    if (argc!=3)
   {
      printf("Se necesitan 3 argumentos\n");
      return -1;
   }
   
   total = atoi(argv[1]); //nro de procesos
   char *str = (char*)argv[2];// arreglo a ordenar
  
   // char *str = "1,2,3,4,5,6,7,8,9,100"; se solo sirve para lectura??
   // char str[] = "1,2,3,4,5,6,7,8,9,100"; //tiene su propia memoria reservada y se puede escribir

    char *duplicate = strdup(str); //duplica cadena, porque strtok modifica la cadena
    const char s[2] = ","; //delimitador

    size = contar(duplicate,s);//contar cantidad de numeros 

   /********reservar y compartir memoria********/
    int *array;
    int id_array_compartido;
    
    int id_variable_compartida;
    int *nro_procesos;

    // int id_archivo_compartido;
    
    nro_procesos = 0;

    asignar(&nro_procesos,1,&id_variable_compartida); //COMPARTIR VARIABLE
    asignar(&array,size, &id_array_compartido);//COMPARTIR ARRAY
    

    /*********Pasar el arreglo str a int**********/
    str_to_int(str,s,array); //pasar de datos de char a int

    //generar array de prueba
	// cargar_array(array,size);

 
    printf("==mapeos==\n");

    /*********Merge sort paralelo****************/

    mergeSort(array,0,size-1,nro_procesos);

    printf("Arreglo ordenado: \n");
	printArray(array,0,size);
    printf("\n");


    /*** Liberar y eliminar memoria compartida ***/
    eliminar(&array, id_array_compartido);
    eliminar(&nro_procesos, id_variable_compartida);


	return 0;
}


/**********funciones de soporte ***********/


/*imprir arreglo*/
void printArray(int A[], int i, int j)
{
    int k;
    for (k = i; k < j; k++){
        printf("%d", A[k]);
        if (k!=(j-1))
        {
            printf(",");
        }
    }

    // printf("\t");
}

/*generar arreglo aleatorio de prueba**/
void cargar_array(int *array, int n){
    
    int j;
    
    for (j = 0; j< n; j++)
    {
        array[j] = rand()%50;
        
    }

    printf("Arreglo original: \n");
    printArray(array,0,n);
    printf("\n");


    return;
}


