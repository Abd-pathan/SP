#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <sys/wait.h>

#define PATH_MAX 4096

typedef int (*FileCheckerFuncPointer)(const char*);
typedef void (*FileIFuncPointer)(const char*, int);
typedef void (*CommandFuncPointer)(const char*, const char*, int, int);

int fileChecker(const char *path);
void fileI(const char *path, int fileDetails);
void myFunc(const char *dir_path, int depth, long size, const char *pattern, int max_depth, int fileDetails, const char *command, const char *commandWithFileList);
void executeCommand(const char *singleCommand, const char *fileListCommand, const char *path, int fileDetails, int useFileListCommand);

FileCheckerFuncPointer fileCheckerP = &fileChecker;
FileIFuncPointer fileIP = &fileI;
CommandFuncPointer commandP = &executeCommand;

int main(int argc, char *argv[])
{
    const char *stDir = ".";
    int fileDetails = 0;
    long size = -1;
    const char *patsr = NULL;
    int max_depth = INT_MAX;
    const char *commandForEachFile = NULL;
    const char *commandWithFileList = NULL; // New option

    int cliInputVar;
    while ((cliInputVar = getopt(argc, argv, "Ss:f:e:E:")) != -1)
    {
        switch (cliInputVar)
        {
            // Existing cases
            case 'S':
                fileDetails = 1;
                break;
            case 's':
                size = atol(optarg);
                break;
            case 'f':
                patsr = optarg;
                if (optind < argc && isdigit(argv[optind][0]))
                {
                    max_depth = atoi(argv[optind]);
                    optind++;
                }
                break;
            case 'e':
                commandForEachFile = optarg;
                break;
            case 'E':
                commandWithFileList = optarg;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc)
    {
        stDir = argv[optind];
    }

    myFunc(stDir, 0, size, patsr, max_depth, fileDetails, commandForEachFile, commandWithFileList);

    return 0;
}

int fileChecker(const char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) == 0)
    {
        return S_ISREG(path_stat.st_mode) ? 1 : 0;
    }
    return 0;
}

void fileI(const char *path, int fileDetails)
{
    struct stat file_stat;
    if (stat(path, &file_stat) == 0)
    {
        printf("File: %s\n", path);
        if (fileDetails)
        {
            printf("Size: %ld bytes\n", (long)file_stat.st_size);
            printf("Permissions: %o\n", file_stat.st_mode & 0777);
            printf("Last Access Time: %s", ctime(&file_stat.st_atime));
        }
        printf("\n");
    }
}

void myFunc(const char *dir_path, int depth, long size, const char *pattern, int max_depth, int fileDetails, const char *command, const char *commandWithFileList)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
    {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        if (fileCheckerP(full_path))
        {
            if ((size < 0 || size >= 0 && size >= (long)entry->d_reclen) &&
                (pattern == NULL || (pattern != NULL && strstr(entry->d_name, pattern) != NULL))
            )
            {
                if (fileDetails)
                {
                    fileIP(full_path, fileDetails);
                }
                else
                {
                    if (entry->d_type != DT_DIR)
                    {
                        for (int i = 0; i < depth; i++)
                        {
                            printf("    ");
                        }
                        printf("%s\n", entry->d_name);

                        if (command)
                        {
                            executeCommand(command, commandWithFileList, full_path, fileDetails, commandWithFileList != NULL);
                        }
                    }
                }
             }
        }
        else
        {
            struct stat path_stat;
            if (stat(full_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
            {
                if (depth < max_depth)
                {
                    for (int i = 0; i < depth; i++)
                    {
                        printf("    ");
                    }
                    printf("%s\n", entry->d_name);
                    myFunc(full_path, depth + 1, size, pattern, max_depth, fileDetails, command, commandWithFileList);
                }
            }
        }
    }
    closedir(dir);
}

void executeCommand(const char *singleCommand, const char *fileListCommand, const char *path, int fileDetails, int useFileListCommand)
{
    if (useFileListCommand)
    {
        pid_t child_pid = fork();
        if (child_pid == -1)
        {
            perror("fork");
            return;
        }

        if (child_pid == 0)
        {
            char full_command[PATH_MAX + 64];
            snprintf(full_command, sizeof(full_command), "%s \"%s\" 2>/dev/null", fileListCommand, path);

            int status = system(full_command);

            if (WIFEXITED(status))
            {
                int exit_status = WEXITSTATUS(status);
                if (exit_status == 0)
                {
                    // printf("Command executed successfully");
                }
                else
                {
                    // printf("Command execution failed");
                }
            }

            //  printf("Exit the child process");
            exit(EXIT_SUCCESS);
        }
        else
        {
            //  printf("This is the parent process");
            int status;
            wait(&status);

            if (WIFEXITED(status))
            {
                int exit_status = WEXITSTATUS(status);
                if (exit_status == 0)
                {
                    // printf("Command executed successfully");
                }
                else
                {
                    //  printf("Command execution failed");
                }
            }
        }
    }
}
