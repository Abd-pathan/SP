#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    FILE* ptr1;
    FILE* myLF;
    char filename1[100];

    printf("\nEnter the name of the file 1:\n ");
    scanf("%s", filename1);
    ptr1 = fopen(filename1, "r");

    myLF = fopen("output.log", "w"); 

    char line[100];
        while (fgets(line, sizeof(line), ptr1) != NULL) {
        printf("Line: %s", line);
    }

    //start time before forking
    time_t startTime;
    time(&startTime);

    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0) { 
        printf("\nThis is the child process, my PID is %d and my parent PID is %d", 
        getpid(),getppid());
        execl("/bin/sh", "sh", "-c", line, NULL);
        perror("execl");
        exit(1);
    } else if (pid < 0) {
        perror("fork");
        exit(1);
    } else {
        wait(&status);
        //End time after the child process completed
        time_t endTime;
        time(&endTime);
        printf("\nStart Time: %s", ctime(&startTime));
        printf("\nEnd Time: %s", ctime(&endTime));
        printf("\nThis is the parent process, my PID is %ld and the child PID is %ld", 
        (long)getpid(), (long)pid);

        // writing into log file
        fprintf(myLF, "%s\t%s\t%s\n", line, ctime(&startTime), ctime(&endTime));

        printf("\nWait for the child process to terminate");
        } 
    fclose(myLF);
    printf("\n[%ld]: Exiting program .....\n", (long)getpid());
    fclose(ptr1);
    return 0;
}
