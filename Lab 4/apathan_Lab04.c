#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE* ptr1;
	char str1[50]; 
    char filename1[100];

    printf("\nEnter the name of the file 1:\n ");
    scanf("%s", filename1);
	ptr1 = fopen(filename1, "a");

	FILE* ptr2;
	char str2[50]; 
	char filename2[100];

    printf("\nEnter the name of the file 2: \n");
    scanf("%s", filename2);
	ptr2 = fopen(filename2, "r");

    
    if(strcmp(filename1,filename2)==0)
    {
        printf("Enter different files");
        exit(1);
    }

    char ch;
    while ((ch = fgetc(ptr2)) != EOF) {
        fputc(ch, ptr1);
    }

	fclose(ptr1);
	fclose(ptr2);
	
    return 0;
}