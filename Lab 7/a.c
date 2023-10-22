#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    FILE* ptr1;
    FILE* logFile;
    char str1[50];
    char filename1[100];

    printf("\nEnter the name of the file 1:\n ");
    scanf("%s", filename1);
    ptr1 = fopen(filename1, "r");

    logFile = fopen("output.log", "w"); // Open the log file for writing

    char line[100];
        while (fgets(line, sizeof(line), ptr1) != NULL) {
        printf("Line: %s", line);
    }

    // Capture the start time before forking
        time_t startTime;
        time(&startTime);

    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0) { /* this is child process */
         printf("\nThis is the child process, my PID is %d and my parent PID is %d\n", 
            getpid(),getppid());
        
        //execl("/usr/bin/uname", "uname","-a", NULL);
        execl("/bin/sh", "sh", "-c", line, NULL);
        // If execl fails, an error will be printed
        perror("execl");
        fclose(ptr1);
        exit(1);
    } else if (pid < 0) {
        // Fork failed
        perror("fork");
        exit(1);
    } else {
        wait(&status); /* wait for the child process to terminate */
        
        // This is the parent process
        // Wait for the child process to complete
        // Capture the end time after the child process completed
            time_t endTime;
            time(&endTime);
            printf("Start Time: %s", ctime(&startTime));
            printf("End Time: %s", ctime(&endTime));
            printf("\nThis is the parent process, my PID is %ld and the child PID is %ld\n", 
            (long)getpid(), (long)pid);

        // Write the log to the log file
            fprintf(logFile, "%s\t%s\t%s\n", line, ctime(&startTime), ctime(&endTime));

          
        printf("\nWait for the child process to terminate\n");
        // if (WIFEXITED(status)) { /* child process terminated normally */
        //     printf("\nChild process exited with status = %d\n", WEXITSTATUS(status));
        // } else { /* child process did not terminate normally */
        //     printf("\nERROR: Child process did not terminate normally!\n");
        //     /* look at the man page for wait (man 2 wait) to 
        //                    determine how the child process was terminated */
        // }

        } 
    fclose(logFile);
    printf("\n[%ld]: Exiting program .....\n", (long)getpid());

    fclose(ptr1);
    return 0;
}
