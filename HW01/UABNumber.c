#include<stdio.h>
int UABNumber()
{
    int n2;
    printf("Enter an Integer: ");
    scanf("%d",&n2);
    int sum=0;
    for(int i=1;i<n2;i++)
    {
        if(n2%i==0) sum+=i;
    }
    return (sum==n2)?1:0;
}
int main()
{
    if(UABNumber())
    {
        printf("true");
    }
    else
    {
        printf("false");
    }
    return 0;
}