
#include <pthread.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>

// codigo para establecer prioridades a los hilos....


void* func()
{
	printf("PRUEBA");

}

int main(int argc, char const *argv[])
{

    pthread_attr_t tattr;
    pthread_t tid;
    int ret;
    int newprio = 20;
    struct sched_param param;

    /* initialized with default attributes */
    ret = pthread_attr_init (&tattr);

    /* safe to get existing scheduling param */
    ret = pthread_attr_getschedparam (&tattr, &param);

    /* set the priority; others are unchanged */
    param.sched_priority = newprio;

    /* setting the new scheduling param */
    ret = pthread_attr_setschedparam (&tattr, &param);

    /* with new priority specified */
    ret = pthread_create (&tid, &tattr, func, NULL); 

    pthread_join(tid, NULL);
    
    return 0;
}
