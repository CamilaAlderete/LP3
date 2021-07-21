#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t child;
    sigset_t mask, omask;
    int status;
    if ((sigfillset(&mask) == -1) ||
            (sigprocmask(SIG_SETMASK, &mask, &omask) == -1))
    {
        perror("Failed to block the signals");
        return 1;
    }
    if ((child = fork()) == -1)
    {
        perror("Failed to fork child");
        return 1;
    }
    if (child == 0)
    {
        /* child code */
        /*nunca va terminar ya que  corre con las senhales bloqueadas
        entre ellas SIGALRM*/
        execl("alarma", "alarma", NULL);
        perror("Child failed to exec");
        return 1;
    } else {
        if (sigprocmask(SIG_SETMASK, &omask, NULL) == -1)  /* parent code */
        {
            perror("Parent failed to restore signal mask");
            return 1;
        }
        status = wait(NULL);
        if (status == -1)
        {
            perror("Parent failed to wait for child");
            return 1;
        }
    }
    return 0;
}
