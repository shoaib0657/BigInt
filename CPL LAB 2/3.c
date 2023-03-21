#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 500

void strRev(char a[])
{
    int n = strlen(a);
    for(int i = 0; i < n/2; ++i)
    {
        char temp = a[i];
        a[i] = a[n - i - 1];
        a[n - i - 1] = temp;
    }
}

void curtail_zeroes(char a[])
{
    int n = strspn(a, "0");
    char s[500];
    int j = 0;
    for(int i = n; a[i] != '\0'; ++i)
    {
        s[j++] = a[i];
    }
    s[j] = '\0';
    strcpy(a, s);
}

int Null(char a[])
{
    if(strlen(a) == 1 && a[0] == 0)
    {
        return 1;
    }

    return 0;
}

int isEqual(char a[], char b[])
{
    if(strcmp(a, b) == 0)
    {
        return 1;
    }

    return 0;
}

int isSamlller(char a[], char b[]) // is a < b
{
    int n = strlen(a), m = strlen(b);
    if(n != m)
    {
        return n < m;
    }

    while(n--)
    {
        if(a[n] != b[n])
        {
            return a[n] < b[n];
        }
    }

    return 0;
}

void modulo(char a[], char b[])
{
    if(Null(b))
    {
        printf("DIVISION BY ZERO IS NOT POSSIBLE");
    }
    else if(isSamlller(a, b))
    {
        printf("%s\n", a);
    }
    else if(isEqual(a, b))
    {
        printf("0\n");
    }
    else
    {
        int i, lgcat = 0, cc;
        int n = strlen(a), m = strlen(b);
        int cat[n];
        for(int i = 0; i < n; i++)
        {
            cat[i] = 0;
        }
        //BigInt t;
        char t[SIZE];
        t[0] = '0';
        // for (i = n - 1; isSmaller(t * 10 + a[i], b); i--){
        //     t *= 10;
        //     t += a[i];
        // }
        while(1)
        {

        }
        for (; i >= 0; i--){
            t = t * 10 + a[i];
            for (cc = 9; cc * b > t;cc--);
            t -= cc * b;
            cat[lgcat++] = cc;
        }
        a = t;
        return a;
    }
}

int main()
{

    return 0;
}