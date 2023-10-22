// -t implement krra hu ismai...abi tak a.c wali mai sai saat kaam hogya hai

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <ctype.h>

#define PATH_MAX 4096  // Define PATH_MAX to a suitable value if not available

typedef int (*FileCheckerFuncPointer)(const char*);
typedef void (*FileIFuncPointer)(const char*, int);
typedef void (*CommandFuncPointer)(const char*, const char*, int);
typedef void (*CommandWithFileListFuncPointer)(const char*, char**, int);

int fileChecker(const char *path);
void fileI(const char *path, int fileDetails);
void myFunc(const char *dir_path, int depth, long size, const char *pattern, int max_depth, int fileDetails, const char *commandForEachFile, const char *commandWithFileList, char fileTypeFilter);
void executeCommandForEachFile(const char *command, const char *path, int fileDetails);
void executeCommandWithFileList(const char *command, char **fileList, int numFiles);

FileCheckerFuncPointer fileCheckerP = &fileChecker;
FileIFuncPointer fileIP = &fileI;

int main(int argc, char *argv[]) {
    const char *stDir = ".";
    int fileDetails = 0;
    long size = -1;
    const char *patsr = NULL;
    int max_depth = INT_MAX;
    const char *commandForEachFile = NULL;
    const char *commandWithFileList = NULL;
    char fileTypeFilter = 'a';  // Default: List all types ('a' stands for 'all')

    int cliInputVar;
    while ((cliInputVar = getopt(argc, argv, "Ss:f:t:e:E:")) != -1) {
        switch (cliInputVar) {
            case 'S':
                fileDetails = 1;
                break;
            case 's':
                size = atol(optarg);
                break;
            case 'f':
                patsr = optarg;
                if (optind < argc && isdigit(argv[optind][0])) {
                    max_depth = atoi(argv[optind]);
                    optind++;
                }
                break;
            case 't':
                if (strcmp(optarg, "f") == 0) {
                    fileTypeFilter = 'f';  // Filter to list regular files only
                } else if (strcmp(optarg, "d") == 0) {
                    fileTypeFilter = 'd';  // Filter to list directories only
                } else {
                    fprintf(stderr, "Error: Invalid -t option. Use 'f' for regular files or 'd' for directories.\n");
                    exit(EXIT_FAILURE);
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

    if (optind < argc) {
        stDir = argv[optind];
    }

    if (commandForEachFile && commandWithFileList) {
        fprintf(stderr, "Error: You cannot specify both -e and -E options simultaneously.\n");
        exit(EXIT_FAILURE);
    }

    myFunc(stDir, 0, size, patsr, max_depth, fileDetails, commandForEachFile, commandWithFileList, fileTypeFilter);

    return 0;
}

int fileChecker(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == 0) {
        return S_ISREG(path_stat.st_mode);
    }
    return 0;
}

void fileI(const char *path, int fileDetails) {
    struct stat file_stat;
    if (stat(path, &file_stat) == 0) {
        printf("File: %s\n", path);
        if (fileDetails) {
            printf("Size: %ld bytes\n", (long)file_stat.st_size);
            printf("Permissions: %o\n", file_stat.st_mode & 0777);
            printf("Last Access Time: %s", ctime(&file_stat.st_atime));
        }
        printf("\n");
    }
}

void myFunc(const char *dir_path, int depth, long size, const char *pattern, int max_depth, int fileDetails, const char *commandForEachFile, const char *commandWithFileList, char fileTypeFilter) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    char *matchingFiles[PATH_MAX]; // Array to store matching file paths
    int numMatchingFiles = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        if (fileTypeFilter == 'f' && !fileCheckerP(full_path)) {
            // Filter to list regular files only, skip if not a regular file
            continue;
        } else if (fileTypeFilter == 'd') {
            // Filter to list directories only, skip if not a directory
            struct stat path_stat;
            if (stat(full_path, &path_stat) == 0 && !S_ISDIR(path_stat.st_mode)) {
                continue;
            }
        }

        if ((size < 0 || size >= 0 && size >= (long)entry->d_reclen) &&
            (pattern == NULL || (pattern != NULL && strstr(entry->d_name, pattern) != NULL))) {
            if (fileDetails) {
                fileIP(full_path, fileDetails);
            } else {
                for (int i = 0; i < depth; i++) {
                    printf("    ");
                }
                printf("%s\n", entry->d_name);
            }

            if (commandForEachFile) {
                executeCommandForEachFile(commandForEachFile, full_path, fileDetails);
            }

            if (commandWithFileList) {
                // Store the matching file paths in the array
                matchingFiles[numMatchingFiles] = strdup(full_path);
                numMatchingFiles++;
            }
        }
    }
    closedir(dir);

   if (commandWithFileList && numMatchingFiles > 0) {
    // Execute the specified command with the list of matching files
    executeCommandWithFileList(commandWithFileList, matchingFiles, numMatchingFiles);
}


    // Free memory allocated for matching file paths
    for (int i = 0; i < numMatchingFiles; i++) {
        free(matchingFiles[i]);
    }
}

void executeCommandForEachFile(const char *command, const char *path, int fileDetails) {
    char full_command[PATH_MAX + 256]; // Add space for command and arguments
    snprintf(full_command, sizeof(full_command), "%s %s", command, path);

    int return_code = system(full_command);
    if (return_code == 0) {
        printf("Command executed successfully for: %s\n", path);
    } else {
        printf("Command execution failed for: %s\n", path);
    }
}

void executeCommandWithFileList(const char *command, char **fileList, int numFiles) {
    if (numFiles == 0) {
        printf("No matching files to execute the command with.\n");
        return;
    }

    // Build the command with the list of files as arguments
    char full_command[PATH_MAX + 256]; // Add space for command and arguments
    strcpy(full_command, command);

    for (int i = 0; i < numFiles; i++) {
        // Add each file path as an argument
        strcat(full_command, " ");
        strcat(full_command, fileList[i]);
    }

    // Redirect stderr to /dev/null
    //strcat(full_command, " 2>/dev/null");

    int return_code = system(full_command);
    if (return_code == 0) {
        printf("Command executed successfully with the list of files.\n");
    } else {
        printf("Command execution failed with the list of files.\n");
    }
}
