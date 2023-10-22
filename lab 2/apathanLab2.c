/* Online C Compiler and Editor */
#include <stdio.h>
int main()
{
    printf("Enter the number of elements in array");
    int N;
    scanf("%d",&N);
    int arr[100];
    printf("Enter array elements");
    for(int i=0;i<N;i++)
    {
    scanf("%d",&arr[i]);
    }
    
    int key,swap;
    for(int i=0;i<N;i++)
    {
    key=i;
    while(key>0 && arr[key-1]>arr[key])
    {
        swap=arr[key];
        arr[key]=arr[key-1];
        arr[key-1]=swap;
        key--;
        
    }
    }
    
    
    printf("Sorted Array-\n");
    for(int i=0;i<N;i++)
    {
    printf("%d",arr[i]);
    }
    return 0;
}