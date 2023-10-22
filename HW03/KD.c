#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    int S_flag;
    int s_flag;
    int f_flag;
    int t_flag;
    int E_flag;
    int e_flag;
    int fileSize;
    char filterTerm[300];
    char fileType[2];
    char fileCommand[200];

} FlagArgs;

// Function to handle files and directories
typedef void FileHandler(char *filePath, char *dirfile, FlagArgs flagArgs, int nestingCount);

void myPrinterFunction(char *filePath, char *dirfile, FlagArgs flagArgs, int nestingCount)
{
    struct stat buf;
    lstat(filePath, &buf);
    char line[100];
    strcpy(line, "");
    strcat(line, dirfile);
    if (flagArgs.S_flag)
    {
        char strsize[10];
        sprintf(strsize, " %d", (int)buf.st_size);
        strcat(line, strsize);
    }
    if (flagArgs.s_flag)
    {
        if (flagArgs.fileSize > (int)buf.st_size)
        {
            strcpy(line, "");
        }
    }
    if (flagArgs.f_flag)
    {
        if (strstr(dirfile, flagArgs.filterTerm) == NULL)
        {
            strcpy(line, "");
        }
    }
    if (flagArgs.t_flag)
    {
        if (strcmp(flagArgs.fileType, "f") == 0)
        {
            if (S_ISDIR(buf.st_mode) != 0)
            {
                strcpy(line, "");
            }
        }
        if (strcmp(flagArgs.fileType, "d") == 0)
        {
            if (S_ISREG(buf.st_mode) != 0)
            {
                strcpy(line, "");
            }
        }
    }
    if (strcmp(line, "") != 0)
    {
        int i = 0;
        for (i = 0; i <= nestingCount; i++)
        {
            printf("\t");
        }
        printf("%s\n", line);
    }
    if (strcmp(flagArgs.fileCommand, "e") == 0)
    {
        // Execute 'e' command here
        // You need to specify what 'e' does and how it's related to tokenization.
    }
    if (strcmp(flagArgs.fileCommand, "E") == 0)
    {
        // Execute 'E' command here
        // You need to specify what 'E' does and how it's related to tokenization.
    }
}

void readFileHierarchy(char *dirname, int nestingCount, FileHandler *fileHandlerFunction, FlagArgs flagArgs)
{
    struct dirent *dirent;
    DIR *parentDir = opendir(dirname);
    if (parentDir == NULL)
    {
        printf("Error opening directory '%s'\n", dirname);
        exit(-1);
    }
    while ((dirent = readdir(parentDir)) != NULL)
    {
        if (strcmp((*dirent).d_name, "..") != 0 && strcmp((*dirent).d_name, ".") != 0)
        {
            char pathToFile[300];
            sprintf(pathToFile, "%s/%s", dirname, ((*dirent).d_name));
            fileHandlerFunction(pathToFile, (*dirent).d_name, flagArgs, nestingCount);
            if ((*dirent).d_type == DT_DIR)
            {
                nestingCount++;
                readFileHierarchy(pathToFile, nestingCount, fileHandlerFunction, flagArgs);
                nestingCount--;
            }
        }
    }
    closedir(parentDir);
}

int main(int argc, char **argv)
{
    int opt = 0;

    FlagArgs flagArgs = {
        .S_flag = 0,
        .s_flag = 0,
        .f_flag = 0,
        .t_flag = 0,
        .E_flag = 0, // Initialize E_flag
        .e_flag = 0, // Initialize e_flag
        .fileSize = 0, // Initialize fileSize
        .filterTerm = "", // Initialize filterTerm
        .fileType = "", // Initialize fileType
        .fileCommand = "" // Initialize fileCommand
    };

    while ((opt = getopt(argc, argv, "Ss:f:t:e:E:")) != -1)
    {
        switch (opt)
        {
        case 'S':
            flagArgs.S_flag = 1;
            break;
        case 's':
            flagArgs.s_flag = 1;
            flagArgs.fileSize = atoi(optarg);
            break;
        case 'f':
            flagArgs.f_flag = 1;
            strncpy(flagArgs.filterTerm, optarg, sizeof(flagArgs.filterTerm) - 1);
            break;
        case 't':
            flagArgs.t_flag = 1;
            strncpy(flagArgs.fileType, optarg, sizeof(flagArgs.fileType) - 1);
            break;
        case 'e':
            flagArgs.e_flag = 1;
            strncpy(flagArgs.fileCommand, optarg, sizeof(flagArgs.fileCommand) - 1);
            break;
        case 'E':
            flagArgs.E_flag = 1;
            strncpy(flagArgs.fileCommand, optarg, sizeof(flagArgs.fileCommand) - 1);
            break;
        }
    }

    if (opendir(argv[argc - 1]) == NULL)
    {
        char defaultdrive[300];
        getcwd(defaultdrive, 300);
        printf("%s\n", defaultdrive);
        readFileHierarchy(defaultdrive, 0, myPrinterFunction, flagArgs);
        return 0;
    }
    printf("%s\n", argv[argc - 1]);
    readFileHierarchy(argv[argc - 1], 0, myPrinterFunction, flagArgs);
    return 0;
}
