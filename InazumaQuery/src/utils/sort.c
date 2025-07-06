#include "InazumaQuery/utils/sort.h"
#include "InazumaQuery/core/core.h"
#include "InazumaQuery/utils/list.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// use quick sort as long as subarray > 16 / 32
// switch to insertion sort when under this treshold
// if recursion depth > 2·log₂(n)⌋ switch to heap sort

// right now, for a 2500 (~ players in ie3) list of ints, times are:
// ~0.00015 seconds in avg conditions
// ~0.003 seconds in worst conditions

// TODO: Implement heapsort
// should allow worst conditions mitigation

typedef ina_list_cmp_fn_t cmp_fn;
typedef ina_list_set_fn_t set_fn;

#define INSERTION_SORT_TRESHOLD 32
#define LS(at) ina_list_at(ls, at)

void hoare_partition(ina_list_t const *ls, cmp_fn compare, set_fn set,
                     int start, int end, int *division);
void quick_sort(ina_list_t const *ls, cmp_fn compare, set_fn set, int start,
                int end, int depth, bool *heapsorted);
void insertion_sort(ina_list_t const *ls, cmp_fn compare, set_fn set, int start,
                    int end);
void heap_sort(ina_list_t const *ls, cmp_fn compare, set_fn set);

void ina_sort(ina_list_t const *ls, cmp_fn compare, set_fn set)
{
    bool heapsorted;
    quick_sort(ls, compare, set, 0, ina_list_count(ls), 0, &heapsorted);
}

void quick_sort(ina_list_t const *ls, cmp_fn compare, set_fn set, int start,
                int end, int depth, bool *heapsorted)
{
    if (end - start < 2) return;

    //  if (depth > 2 * log2(len))
    //  {
    //      heap_sort(arr, len);
    //      *heapsorted = true;
    //      return;
    //  }

    if (end - start <= INSERTION_SORT_TRESHOLD)
    {
        insertion_sort(ls, compare, set, start, end);

        return;
    }
    int div;

    hoare_partition(ls, compare, set, start, end, &div);

    quick_sort(ls, compare, set, start, div + 1, depth + 1, heapsorted);

    if (*heapsorted) return;

    quick_sort(ls, compare, set, div + 1, end, depth + 1, heapsorted);
}

void hoare_partition(ina_list_t const *ls, cmp_fn compare, set_fn set,
                     int start, int end, int *div)
{
    void *piv = LS(start);
    int i = (int)start - 1;
    int j = (int)end;

    while (1)
    {
        do
        {
            i++;
        } while (compare(LS(i), piv) < 0);

        do
        {
            j--;
        } while (compare(LS(j), piv) > 0);

        if (i >= j)
        {
            *div = j;
            break;
        }

        void *temp = malloc(ina_list_sizeof_element(ls));
        memcpy(temp, LS(i), ina_list_sizeof_element(ls));

        set(LS(i), LS(j));
        set(LS(j), temp);
    }
}

void insertion_sort(ina_list_t const *ls, cmp_fn compare, set_fn set, int start,
                    int end)
{
    for (int i = start + 1; i < end; i++)
    {
        int j = i;
        while (j > start && compare(LS(j - 1), LS(j)) > 0)
        {
            void *temp = malloc(ina_list_sizeof_element(ls));
            memcpy(temp, LS(j), ina_list_sizeof_element(ls));

            set(LS(j), LS(j - 1));
            set(LS(j - 1), temp);
            j--;
        }
    }
}

void heap_sort(ina_list_t const *ls, cmp_fn compare, set_fn set)
{
    INA_NOT_IMPLEMENTED;
}