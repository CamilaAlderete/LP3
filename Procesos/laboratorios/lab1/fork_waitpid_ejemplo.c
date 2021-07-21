#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
/*https://stackoverflow.com/questions/20465039/what-does-wexitstatusstatus-return*/

int main(int argc, char **argv)
{
    printf("--beginning of program\n");

    int counter = 0;
    int estado;
    pid_t pid = fork();
	
    if (pid == 0)
    {
        // child process
            
    	printf("child process id: %d\n", getpid());
            sleep(3);
    	printf("end child\n");
    	return 5;
    }
    else if (pid > 0)
    {
        // parent process
    	printf("parent process id: %d, wait to child: %d\n", getpid(), pid);
        
        waitpid(pid,&estado,0);//el padre espera al hijo con cierto pid, el cual retornara un estado
    	printf("end parent, child status: %d\n", WEXITSTATUS(estado));  /*si no se coloca WEXITSTATUS, retorna 1280...*/

        /*If the WIFEXITED macro indicates that the child process exited normally, the WEXITSTATUS macro 
        returns the exit code specified by the child. If the WIFEXITED macro indicates that the child process did not exit normally, the value returned by the WEXITSTATUS macro has no meaning.*/
       
    }
    else
    {
        // fork failed
        printf("fork() failed!\n");
        return 1;
    }
}
