#include "inazuma/utils/sort.h"
#include "inazuma/core/core.h"
#include "inazuma/utils/list.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// use quick sort as long as subarray > 16 / 32
// switch to insertion sort when under this treshold
// if recursion depth > 2·log₂(n)⌋ switch to heap sort

// right now, for a 2500 (~ players in ie3) ls of ints, times are:
// ~0.00015 seconds in avg conditions
// ~0.003 seconds in worst conditions

// TODO: IMPLEMENT HEAPSORT
// should allow worst conditions mitigation

typedef InaListElementGetComparatorFn ExtractorFn;
typedef InaListElementSetFn SetterFn;

#define INSERTION_SORT_TRESHOLD 32
#define LS(at) ina_list_at(ls, at)

void hoare_partition(InaList const *ls, ExtractorFn extract, SetterFn set,
                     int start, int end, int *division);
void quick_sort(InaList const *ls, ExtractorFn extract, SetterFn set, int start,
                int end, int insertion_th, bool heapsort, int depth,
                bool *heapsorted);
void insertion_sort(InaList const *ls, ExtractorFn extract, SetterFn set,
                    int start, int end);
void heap_sort(InaList const *ls, ExtractorFn extract, SetterFn set);

void __ina_sort(InaList const *ls, ExtractorFn extract, SetterFn set,
                InaSortDir sort_dir, int insertion_th, bool heapsort)
{
    bool heapsorted;
    quick_sort(ls, extract, set, 0, ina_list_count(ls), insertion_th, heapsort,
               0, &heapsorted);

    if (sort_dir == INA_SORT_DIR_DESCENDING)
    {
        __ina_reverse(ls, extract, set);
    }
}

void quick_sort(InaList const *ls, ExtractorFn extract, SetterFn set, int start,
                int end, int insertion_th, bool heapsort, int depth,
                bool *heapsorted)
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
        insertion_sort(ls, extract, set, start, end);

        return;
    }
    int div;

    hoare_partition(ls, extract, set, start, end, &div);

    quick_sort(ls, extract, set, start, div + 1, insertion_th, heapsort,
               depth + 1, heapsorted);

    if (*heapsorted) return;

    quick_sort(ls, extract, set, div + 1, end, insertion_th, heapsort,
               depth + 1, heapsorted);
}

void hoare_partition(InaList const *ls, ExtractorFn extract, SetterFn set,
                     int start, int end, int *div)
{
    int piv = extract(LS(start));
    int i = (int)start - 1;
    int j = (int)end;

    while (1)
    {
        do
        {
            i++;
        } while (extract(LS(i)) < piv);

        do
        {
            j--;
        } while (extract(LS(j)) > piv);

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

void insertion_sort(InaList const *ls, ExtractorFn extract, SetterFn set,
                    int start, int end)
{
    for (int i = start + 1; i < end; i++)
    {
        int j = i;
        while (j > start && extract(LS(j - 1)) > extract(LS(j)))
        {
            void *temp = malloc(ina_list_sizeof_element(ls));
            memcpy(temp, LS(j), ina_list_sizeof_element(ls));

            set(LS(j), LS(j - 1));
            set(LS(j - 1), temp);
            j--;
        }
    }
}

INA_API void __ina_reverse(InaList const *ls, ExtractorFn extract, SetterFn set)
{
    size_t len = ina_list_count(ls);
    size_t last = len - 1;
    size_t half = len / 2;

    for (size_t i = 0; i < half; ++i)
    {
        void *temp = malloc(ina_list_sizeof_element(ls));
        memcpy(temp, LS(i), ina_list_sizeof_element(ls));

        set(LS(i), LS(last - i));
        set(LS(last - i), temp);
    }
}
void heap_sort(InaList const *ls, ExtractorFn extract, SetterFn set)
{
    INA_NOT_IMPLEMENTED;
}