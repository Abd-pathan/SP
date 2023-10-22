#include<stdio.h>

int smallerThanIndex(int* arr,int length)
{
    int count=0;
    for(int i=0;i<length;i++)
    {
        if(i>arr[i])
        {
            count++;
        }
    }
    return count;
}
int main()
{
    printf("Enter the number of Elements in an Array: ");
    int len;
    scanf("%d",&len);
    int arr[100];
    for(int i=0;i<len;i++)
    {
        scanf("%d",&arr[i]);
    }
    printf("Answer : %d",smallerThanIndex(arr,len));
    return 0;
}