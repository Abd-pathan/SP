#include<stdio.h>
#include<string.h>
int isPrime(int n)
{
    for(int i=2;i<n;i++)
    {
        if(n%i==0)
        {
            return 0;
        }
        return 1;
    }
}

int checkPowerOf2(int n)
{
    int count=0;
    while(n>0)
    {
        count+=n%2;
        n=n/2;
    }
    return (count==1)?1:0;
}

char* numberTransformer(int n)
{
    char ch[100];
    if(isPrime(n)&&(n!=3||n!=5))
    {
        return "Go Blazer";
    }
    else if(checkPowerOf2(n))
    {
        if(n==1||n==2)
        {
            sprintf(ch,"%d",n);
            return ch;
        }
        int sum=n;
        for(int i=n-1;i>1;)
        {
            if(isPrime(i))
            {
                sum+=i;
                sprintf(ch,"%d",sum);
                return ch;
            }
        }
    }
    else if(n%3==0)
    {//if sum of digits of n is divisible by 3 and then n is also divisible by 5.
        if(n%5==0)
        {
            return "UAB CS 332&532";
        }
        else
        {
            return "CS";
        }
    }
    else
    {
        int sum=0,b;
        while(n>0)
        {
            b=n%10;
            sum+=b*b;
            n=n/10;
        }
        sprintf(ch,"%d",sum);
        return ch;
    }

}

int main()
{
    printf("Enter an Positive Number: ");
    int n;
    scanf("%d",&n);
    if(n<=0)
    {
        printf("Invalid Number.");
    }
    else{
        printf("%s",numberTransformer(n));
    }
    return 0;
}