#include<stdio.h>
int reverseNum(int n)
{
    int n2=0;
    while(n>0)
    {
        n2=n2*10+(n%10);
        n/=10;
    }
    return n2;
    
}
int main()
{
    int n;
    printf("Enter a Positive Number: ");
    scanf("%d",&n);
    printf("%d",reverseNum(n));
    return 0;
}