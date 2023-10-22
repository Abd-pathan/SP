#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

pid_t pid;

static void funcSigint(int userSignal) {
    if (userSignal == SIGINT) {
        kill(pid, userSignal);
        printf("Child process is suspended\n");
        printf("Parent process is waiting\n");
    }
}
static void funcSigtstp(int userSignal) {
    if (userSignal == SIGTSTP) {
        printf("Child process is suspended\n");
        printf("Parent process is waiting\n");
        kill(pid, userSignal);
    }
}
static void funcSigquit(int userSignal) {
    if (userSignal == SIGQUIT) {
        printf("Terminating Program...\n");
        kill(pid, userSignal);
        exit(0);
    }
}

int main(int argc, char **argv) {
    int status;

    pid = fork();
    if (pid == 0) {  
    printf("Child process is running\n");
    exit(0);
    
    } else if (pid > 0) { 
       if (signal(SIGINT, funcSigint) == SIG_ERR) {
            exit(-1);
        }
        if (signal(SIGTSTP, funcSigtstp) == SIG_ERR) {
            exit(-1);
        }
        if (signal(SIGQUIT, funcSigquit) == SIG_ERR) {
            exit(-1);
        }
        printf("Parent process is waiting\n");

        while (1) {
            sleep(1);  
        }

        wait(&status); 
        if (WIFEXITED(status)) {
            printf("Child process exited with status = %d\n", WEXITSTATUS(status));
        } else { 
            printf("Child process did not terminate normally!\n");
            }
    } else {
        perror("fork"); 
        exit(EXIT_FAILURE);
    }
    printf("[%ld]: Exiting program .....\n", (long)getpid());
    return 0;
}
