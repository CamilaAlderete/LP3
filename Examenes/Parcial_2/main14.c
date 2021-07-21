#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h> //boleano

// compilar  cc main14.c -o a -pthread

//nro max. de vehiculos que pueden cruzar para luego cambiar de sentido
#define inanicion 4
//longitud del puente
#define long_puente 3

//#define max_blancos 99
//#define max_rojos 99
//salir del programa
bool exit_ = false;

int blancos_que_pasaron = 0;
int rojos_que_pasaron= 0;

int blancos_en_espera=0;
int rojos_en_espera=0;

int turno_blanco = 0;
int turno_rojo=0;

//codigo del emoji..... para guardar en una variable
//🤺
char* emoji_blanco = "\360\237\244\272\0";

//💃
char* emoji_rojo = "\360\237\222\203\0";
char* vacio="_\0";

//para que los autos esperen en la entrada del puente e ingresen de a uno
pthread_cond_t var_margen_izq;
pthread_cond_t var_margen_der;

pthread_mutex_t mutex_margen_derecho;
pthread_mutex_t mutex_margen_izquierdo; 

//para las tres partes del puente
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;


int puente[long_puente] = {0};
char* puente_emoji[long_puente];

bool izq_der = true;
bool der_izq = false;

bool puente_libre(){

    //verificar que el puente este sin vehiculos
    for (int i = 0; i < long_puente; i++)
    {
        if (puente[i] != 0)
        {
            return false;
        }   
    }
    return true; 
}


void print_puente(){

    printf("\t\t\t      ☁      ⁣⛅      ☁ \n");
    printf("\t\t⁣\t\t☁        🚁          ☁\n");
    printf("\t\t\t  🏢🏢🌴🏫🌳⛪🏥🏪🌴🏤🌴🏢🌳🌳\n");
    printf("\t\t\t🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊\n");

    printf("\t");
    for (int i = 0; i < long_puente; i++)
    {
       printf("\t\t(%d)",puente[i]);
       
    }
    printf("\n");

    printf("\t");
    for (int i = 0; i < long_puente; i++)
    {
       printf("\t\t%s",puente_emoji[i]);
       
    }
    printf("\n");
    printf("\t\t\t🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊🌊\n");

}

void print_blanco_en_espera(){
    int i;
    for( i = 0; i < blancos_en_espera; i++)
    {
        printf("⚪️");
        //printf("%s ",emoji_blanco);
        
    }
    printf("\n");
    
}


void print_rojo_en_espera(){
    int i;
    for ( i = 0; i < rojos_en_espera; i++)
    {
        //printf("%s ",emoji_rojo);
        printf("🔴");
    }
    printf("\n");
}

void print_rojo_pasaron(){
    int i;
    for ( i = 0; i < rojos_que_pasaron; i++)
    {
        //printf("%s ",emoji_rojo);
        printf("🔴");

    }
    printf("\n");
}

void print_blanco_pasaron(){
    int i;
    for ( i = 0; i < blancos_que_pasaron; i++)
    {
        //printf("%s ",emoji_blanco);
        printf("⚪️");
    }
    printf("\n");
}

//agente de transito 👮
void* swap(void* arg){

    sleep(2);

    while (exit_==false)
    {
        if ( puente_libre()==true) //puente libre
        {
            int nro_senales;
            //iniciar transito....
            if (izq_der==true) //sentido permitido --->
            {
                nro_senales=1;
                //senales enviadas menor o igual inanicion y haya rojos en espera
                while ( (nro_senales<= inanicion) && rojos_en_espera>0)
                {
                    //permitir pase de autos cada un segundo
                    pthread_cond_signal(&var_margen_izq);
                    sleep(1);
                    nro_senales++;
                }


                //cambio de sentido
                izq_der=false;
                der_izq=true;
                

            }else if (der_izq==true)// sentido permitido <----
            {
                nro_senales=1;
                //senales enviadas menor o igual inanicion y que haya blancos en espera
                while ( (nro_senales<= inanicion) && blancos_en_espera>0)
                {
                    //permitir pase de autos cada un segundo
                    pthread_cond_signal(&var_margen_der);
                    sleep(1);
                    nro_senales++;
                }

                
                //cambio de sentido
                der_izq=false;
                izq_der=true;

            }

            sleep(2);


        }else
        {
            //esperar a que puente se libere
            usleep(1500000);
            //sleep(2);
        }
        
    }
    

}

// <=
void* blanco(void* arg){
    int nro_auto;
        
    //para que los hilos esperen...
    pthread_mutex_lock(&mutex_margen_derecho);
    pthread_cond_wait(&var_margen_der, &mutex_margen_derecho); //desbloquea, espera senal
    pthread_mutex_unlock(&mutex_margen_derecho);
    
    turno_blanco++;
    nro_auto = turno_blanco;

    blancos_en_espera--;
     
    pthread_mutex_lock(&mutex3); 
        puente[2]= nro_auto; 
        puente_emoji[2]=emoji_blanco;
        sleep(1);
        puente[2]=0;
        puente_emoji[2]=vacio;
    pthread_mutex_unlock(&mutex3);

    pthread_mutex_lock(&mutex2);
        puente[1]= nro_auto; 
        puente_emoji[1]=emoji_blanco;
        sleep(1);
        puente[1]=0;
        puente_emoji[1]=vacio;
    pthread_mutex_unlock(&mutex2);

    pthread_mutex_lock(&mutex1);
        puente[0]= nro_auto;
        puente_emoji[0]=emoji_blanco;
        sleep(1);
        puente[0]=0;
        puente_emoji[0]=vacio;
    pthread_mutex_unlock(&mutex1);
    
    blancos_que_pasaron++;
   
    pthread_exit(NULL);
}

//=>
void* rojo(void* arg){
    int nro_auto;
        
    //para que los hilos esperen senal del "agente de transito"
    pthread_mutex_lock(&mutex_margen_izquierdo);
    pthread_cond_wait(&var_margen_izq, &mutex_margen_izquierdo); //desbloquea, espera senal
    pthread_mutex_unlock(&mutex_margen_izquierdo);

    turno_rojo++;
    nro_auto = turno_rojo;
    
    rojos_en_espera--;

    pthread_mutex_lock(&mutex1);
        puente[0]= nro_auto; 
        puente_emoji[0]=emoji_rojo;
        sleep(1);
        puente[0]=0;
        puente_emoji[0]=vacio;
    pthread_mutex_unlock(&mutex1);
    
    pthread_mutex_lock(&mutex2);
        puente[1]= nro_auto;
        puente_emoji[1]=emoji_rojo;
        sleep(1);
        puente[1]=0;
        puente_emoji[1]=vacio;
    pthread_mutex_unlock(&mutex2);

    pthread_mutex_lock(&mutex3); 
        puente[2]= nro_auto;
        puente_emoji[2]=emoji_rojo;
        sleep(1);
        puente[2]=0;
        puente_emoji[2]=vacio;
    pthread_mutex_unlock(&mutex3);
        
    rojos_que_pasaron++;

    pthread_exit(NULL); //por si acaso?

}


void menu1(){
    printf("1. Agregar auto en el margen izquierdo de puente: > \n");
    printf("2. Agregar auto en el margen derecho de puente: < \n");
    printf("3. Iniciar transito: s \n");
    printf("4.Salir: ctrl + c \n");
    puts("Se puede ingresar los vehiculos de la forma:\n > > > < < >\no tambien...\n >\n>\n>\n<\n<\n>\n");

}

void menu2(){
    printf("1. Agregar auto en el margen izquierdo: > \n");
    printf("2. Agregar auto en el margen derecho: < \n");
    printf("3.Salir: x \n");
}

void print_estado(){
    printf("\n");
    //autos rojos (de izquierda a derecha)
    printf("%s [Destino: derecha]\n\t⏰ En espera: %d  ",emoji_rojo,rojos_en_espera);
    print_rojo_en_espera();
    printf("\t✔️ Cruzaron: %d  ",rojos_que_pasaron);
    print_rojo_pasaron();
    

    //autos blancos (derecha a izquierda)
    printf("%s [Destino: izquierda]\n\t⏰ En espera: %d  ",emoji_blanco,blancos_en_espera);
    print_blanco_en_espera();
    printf("\t✔️ Cruzaron: %d ",blancos_que_pasaron);
    print_blanco_pasaron();
    
}



void* print_vista(void* arg){
    
    while (exit_ == false)
    {
      system("clear");
      puts("-----------------------------------------------------------------------\n");
      print_puente();
      puts("-----------------------------------------------------------------------\n");
      printf("\n");
      print_estado();
      puts("-----------------------------------------------------------------------\n");
      menu2();
      sleep(1);

    }
    
}



int main(int argc, char const *argv[])
{
    bool iniciado = false;

    char cmd[1024]; //instrucciones por consola
    
    //inicializar puente_emoji
    for (int i = 0; i < long_puente; i++)
    {
        puente_emoji[i]=vacio;
    }
    

    //mutex para cada seccion del puente
    pthread_mutex_init(&mutex1, NULL); 
	pthread_mutex_init(&mutex2, NULL); 
	pthread_mutex_init(&mutex3, NULL); 

    //inicializacion de los mutex de ambos margenes del puente
	pthread_mutex_init(&mutex_margen_derecho, NULL);
	pthread_mutex_init(&mutex_margen_izquierdo, NULL); 

    //inicializacion de variables de condicion de ambos margenes del puente
    pthread_cond_init(&var_margen_izq, NULL);
    pthread_cond_init(&var_margen_der, NULL);


    //pthread_t blancos[max_blancos]; //<=
    //pthread_t rojos[max_rojos]; //=>

    system("clear");
    menu1();


    while(fscanf(stdin, "%s", cmd) != EOF)
    {
        
        if(strcmp(cmd, "<") == 0)
        {
            
            blancos_en_espera++;
            pthread_t *t;
            t = (pthread_t *) malloc(sizeof(pthread_t));
            pthread_create(t, NULL, &blanco,NULL);
            //pthread_join(*t,NULL); //no..        
        }
        else if(strcmp(cmd, ">") == 0) //auto rojo
        {

            rojos_en_espera++;

            pthread_t *t;
            t = (pthread_t *) malloc(sizeof(pthread_t));
            pthread_create(t, NULL, &rojo,NULL);

        }else if ( (strcmp(cmd, "s") == 0) && (iniciado==false))
        {
            system("clear");

            //visualizar vehiculos en pantalla
            pthread_t vista;
            pthread_create(&vista,NULL,&print_vista,NULL);

            //iniciar transito
            iniciado = true;
            pthread_t* ini = (pthread_t *) malloc(sizeof(pthread_t));
            pthread_create(ini, NULL, &swap, NULL);
        }       
        else if(strcmp(cmd, "x") == 0){

            
            exit_=true; 

            //destruir todos los mutex y variables de condicion
            pthread_mutex_destroy(&mutex_margen_derecho);
            pthread_mutex_destroy(&mutex_margen_izquierdo);
        

            pthread_mutex_destroy(&mutex1);
            pthread_mutex_destroy(&mutex2);
            pthread_mutex_destroy(&mutex3);
           
            //cuando hay procesos pendientes(), no se pueden destruir las var. de cond.
            //esto no funciona: > > > < < x (crear procesos y terminar sin iniciar)
            pthread_cond_destroy(&var_margen_der);
            pthread_cond_destroy(&var_margen_izq);

            system("clear");
            exit(0);      
            
        }

    }
    
    return 0;
}
