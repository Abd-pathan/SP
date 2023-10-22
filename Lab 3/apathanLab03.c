#include <stdio.h>
#include <string.h>

void read(char *arr1)
{
    printf("Enter a string: ");
    scanf("%s", arr1);
}

void sort(char *arr1) 
{
    int length = strlen(arr1);
    for (int i = 1; i < length; i++) 
    {
        int key = i;
        while(key>0 && arr1[key-1]>arr1[key])
        {
        char swap=arr1[key];
        arr1[key]=arr1[key-1];
        arr1[key-1]=swap;
        key--;
        }
    }
}

void display(char *arr1)
{
    printf("Sorted String: %s\n", arr1);
}

int main()
{
    char arr1[100];
    read(arr1);
    sort(arr1);
    display(arr1);
    return 0;
}
