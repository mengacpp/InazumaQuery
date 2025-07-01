#include "inazuma/utils/sort.h"
#include "inazuma/core.h"

#include <stddef.h>
#include <stdio.h>

// use quick sort as long as subarray > 16 / 32
// switch to insertion sort when under this treshold
// if recursion depth > 2·log₂(n)⌋ switch to heap sort

// right now, for a 2500 (~ players in ie3) list of ints, times are:
// ~0.00015 seconds in avg conditions
// ~0.003 seconds in worst conditions

// TODO: IMPLEMENT HEAPSORT
// should allow worst conditions mitigation

#define INSERTION_SORT_TRESHOLD 32

void hoare_partition(int arr[], int start, int end, int *division);
void quick_sort(int arr[], int start, int end, int len, int insertion_th,
                bool heapsort, int depth, bool *heapsorted);
void insertion_sort(int arr[], int start, int end);
void heap_sort(int arr[], int len);

void __ina_sort(int arr[], int len, int insertion_th, bool heapsort)
{
    bool heapsorted;
    quick_sort(arr, 0, len, len, insertion_th, heapsort, 0, &heapsorted);
}

void quick_sort(int arr[], int start, int end, int len, int insertion_th,
                bool heapsort, int depth, bool *heapsorted)
{
    if (end - start < 2) return;

    //  if (depth > 2 * log2(len))
    //  {
    //      heap_sort(arr, len);
    //      *heapsorted = true;
    //      return;
    //  }

    if (end - start <= insertion_th)
    {
        insertion_sort(arr, start, end);

        return;
    }
    int div;

    hoare_partition(arr, start, end, &div);

    quick_sort(arr, start, div + 1, len, insertion_th, heapsort, depth + 1,
               heapsorted);

    if (*heapsorted) return;

    quick_sort(arr, div + 1, end, len, insertion_th, heapsort, depth + 1,
               heapsorted);
}

void hoare_partition(int arr[], int start, int end, int *div)
{
    int piv = arr[start];
    int i = (int)start - 1;
    int j = (int)end;

    while (1)
    {
        do
        {
            i++;
        } while (arr[i] < piv);

        do
        {
            j--;
        } while (arr[j] > piv);

        if (i >= j)
        {
            *div = j;
            break;
        }

        int temp = arr[i];

        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void insertion_sort(int arr[], int start, int end)
{
    for (int i = start + 1; i < end; i++)
    {
        int j = i;
        while (j > start && arr[j - 1] > arr[j])
        {
            int tmp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = tmp;
            j--;
        }
    }
}


void heap_sort(int arr[], int len)
{
    INA_NOT_IMPLEMENTED;
}