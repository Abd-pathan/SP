int main()


{



    int a = 10, b = 25;



    a = b++ + a++;



    b = ++b + ++a;



    printf("%d %d n", a, b);



}