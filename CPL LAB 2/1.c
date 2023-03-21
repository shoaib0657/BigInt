#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 100000

void static_memory()
{
    static int arr[SIZE];
}

void stack_memory()
{
    int arr[SIZE];
}

void heap_memory()
{
    int *arr;
    arr = (int*)malloc(sizeof(int) * SIZE);
}

int main()
{
    clock_t start, end;
    
     // Test static memory
    start = clock();
    for(int i = 0; i < 200000; i++)
    {
        static_memory();
    }
    end = clock();
    printf("Static memory: %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);
    
    //test stack memory
    start = clock();
    for (int i = 0; i < 200000; i++) 
    {
        stack_memory();
    }
    end = clock();    
    printf("Stack memory: %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);
    
    // Test heap memory
    start = clock();
    for (int i = 0; i < 200000; i++) 
    {
        heap_memory();
    }
    end = clock();
    printf("Heap memory: %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);

    return 0;
}