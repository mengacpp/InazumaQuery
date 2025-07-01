#include "inazuma/utils/sort.h"
#include <ctime>
#include <iostream>
#include <ostream>
#include <stddef.h>

#include <stdlib.h>
#include <time.h>

#define N 2500
int arr1[N];
int arr2[N];

void shuffle(int *a, size_t n)
{
    for (size_t i = n - 1; i > 0; --i)
    {
        size_t j = rand() % (i + 1);
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}

#define lengthof(arr) sizeof(arr) / sizeof(*arr)

int sort(int arr[], size_t len, int insertion_treshold, bool heapsort)
{
    std::cout << "start: " << /* print_array(arr, len) << */ "\n" << std::flush;

    clock_t start = clock();
    __ina_sort(arr, len, insertion_treshold, heapsort);
    clock_t end = clock();

    // std::cout << "result: " << print_array(arr, len) << "\n";
    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to sort (with treshold at %i): %.6f seconds\n",
           insertion_treshold, cpu_secs);

    std::cout << std::flush;

    return 1;
};

int main()
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < N; i++)
        arr1[i] = i + 1;
    shuffle(arr1, N);

    srand((unsigned)time(NULL));
    for (int i = 0; i < N; i++)
        arr2[i] = i + 1;
    shuffle(arr2, N);

    if (!sort(arr1, lengthof(arr1), 32, true)) return 1;

    if (!sort(arr1, lengthof(arr1), 32, true)) return 1;

    return 0;
}