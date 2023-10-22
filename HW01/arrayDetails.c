#include<stdio.h>

void arrayDetails(int* arr,int length)
{
    int max,min,maxId,minId;double sum;
    max=min=arr[0];maxId=minId=0;sum=0;
    for(int i=0;i<length;i++)
    {
        if(arr[i]>max)
        {
            max=arr[i];
            maxId=i;
        }
        if(arr[i]<min)
        {
            min=arr[i];
            minId=i;
        }
        sum+=arr[i];
    }
    double output[6];
    output[0]=length;output[1]=min;output[2]=minId;
    output[3]=sum/length;output[4]=max;output[5]=maxId;
    //printing array.
    printf("[%.0lf, %.0lf, %.0lf, %.2lf, %.0lf, %.0lf]",output[0],output[1],output[2],output[3],output[4],output[5]);

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
    arrayDetails(arr, len);
    return 0;
}