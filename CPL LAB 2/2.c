#include<stdio.h>
#include<time.h>

// function to reference the 2D array through subscripts.
void my_func_with_only_subscript()
{
    int arr[15][5] = {0};
    for(int j = 0; j < 5 ; j++)
    {
        for(int i = 0; i < 15; i++)
        {
            arr[i][j]; //no operation is done, only accessing the element
        }
    }
    return;
}

// function to reference the 2D array through pointers.
void my_func_with_pointers()
{
    int arr[15][5] = {0};
    for(int j = 0; j < 5 ; j++)
    {
        for(int i = 0; i < 15; i++)
        {
            *(*(arr+i)+j); //no operation is done, only accessing the element
        }
    }
    return;
}

int main()
{
    clock_t start, end;
    double cpu_time_used;
    
    //starting the cpu clock.
    start = clock();
    for(int i = 0; i < 10000; i++)
    {
        my_func_with_only_subscript();
    }
    // closing the cpu clock.
    end = clock();

    //calculating time elapsed in secs.
    cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC;

    printf("Time taken with subscript references::%f\n", cpu_time_used);

    // starting the cpu clock
    start = clock();
    for(int i = 0; i < 10000; i++)
    {
        my_func_with_pointers();
    }
    // closing the cpu clock.
    end = clock();

    // calculating the time elapsed in array reference through pointers.
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Time taken with pointers::%f\n",cpu_time_used);

    return 0;
}