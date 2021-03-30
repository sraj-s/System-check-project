// Name : Sambeg Raj Subedi 
// Date: 03/05/2021



#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    pid_t child1, child2;
    int fd;
    /* -------------------------- for pipes ----------------------------- */  

    int creating_pipe[2];
    char buff[1024];
    ssize_t numb_read = -1;

    if (pipe(creating_pipe) == -1)                           // Condition for error. 
    {
        fprintf(stderr, "We are unable to create a pipe\n");
        return 1;
    }

    printf("Pipes has been created. \n");

    /* -------------------------- child1 process--------------------------- */

    child1 = fork();                                         // Creating a child process using fork. 
    if (child1 < 0)                                          // Condition failed to fork for child1. 
    {
        fprintf(stderr, "Fork faild for child1\n");
        return 1;
    }

    else if (child1 == 0)
    {
        printf("child1  active ");

        if (close(creating_pipe[0]) == -1)                
        {
            fprintf(stderr, "Error on closing read-end of the pipe\n");
            return 1;
        }

        dup2(creating_pipe[1], 1);
        execlp("ls", "ls", "-F", NULL);

        if (close(creating_pipe[1]) == -1)
        {
            fprintf(stderr, "Error on closing Write-end of the pipe\n");
            return 1;
        }
    }
    /* -------------------------- child2 process---------------------------- */
    else
    {
        child2 = fork();                                      // Creating a child process using fork.
        if (child2 < 0)                                       // Condition, failed to fork for child2.
        {
            fprintf(stderr, "Fork faild for child2\n");
            return 1;
        }
        else if (child2 == 0)
        {
            if(close(creating_pipe[1]) == -1) {
                fprintf(stderr, "Error on closing read-end of the pipe\n");
                return 1;
            }

            dup2(creating_pipe[0], 0);
            execlp("nl", "nl", NULL);

            if(close(creating_pipe[0]) == -1) {
                fprintf(stderr, "Error on closing Write-end of the pipe\n");
                return 1;
            }
        }

/* -------------------------- Parent process--------------------------- */

        else
        {           
            if (wait(NULL) == -1)
            {
                printf("Error encountered\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    printf("Work Completed\n");
    return 0;
}