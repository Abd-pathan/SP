#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024

int compareStrings(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

int main() 
{
    FILE *inputFile = fopen("listings.csv", "r");
    if (inputFile == NULL) 
    {
        perror("Error opening input file");
        return 1; 
    }

    char line[MAX_LINE_LENGTH];
    char **field1s = NULL; 
    char **field2s = NULL; 
    int max_fields = 100; 
    int field1_count = 0;
    int field2_count = 0;

    field1s = (char **)malloc(max_fields * sizeof(char *));
    field2s = (char **)malloc(max_fields * sizeof(char *));

    if (field1s == NULL || field2s == NULL) {
        perror("Memory allocation failed");
        fclose(inputFile);
        return 1;
    }

    while (fgets(line, sizeof(line), inputFile)) {
        if (field1_count >= max_fields || field2_count >= max_fields) {
            max_fields *= 2; 
            field1s = (char **)realloc(field1s, max_fields * sizeof(char *));
            field2s = (char **)realloc(field2s, max_fields * sizeof(char *));
            if (field1s == NULL || field2s == NULL) {
                perror("Memory allocation failed");
                fclose(inputFile);
                return 1;
            }
        }

        char *token = strtok(line, ",");
        int field_index = 0;

        while (token != NULL) {
            if (field_index == 2) { 
                field1s[field1_count] = strdup(token);
                field1_count++;
            } else if (field_index == 8) { 
                field2s[field2_count] = strdup(token);
                field2_count++;
            }
            token = strtok(NULL, ",");
            field_index++;
        }
    }

    fclose(inputFile); 

    qsort(field1s, field1_count, sizeof(char *), compareStrings);

   FILE *outputFileField1 = fopen("c1.csv", "w");
    if (outputFileField1 == NULL) {
        perror("Error opening field1 output file");
        return 1; 
        }
    for (int i = 0; i < field1_count; i++) {
        fputs(field1s[i], outputFileField1);
        fputs("\n", outputFileField1); // Add newline
        free(field1s[i]); 
        }
    fclose(outputFileField1); 
    free(field1s); 
    

    qsort(field2s, field2_count, sizeof(char *), compareStrings);

    FILE *outputFileField2 = fopen("c2.csv", "w");
    if (outputFileField2 == NULL) {
        perror("Error opening field2 output file");
        return 1; 
        }
    for (int i = 0; i < field2_count; i++) {
        fputs(field2s[i], outputFileField2);
        fputs("\n", outputFileField2); // Add newline
        free(field2s[i]); 
        }
    fclose(outputFileField2);
    free(field2s); 
    
    return 0;
}
