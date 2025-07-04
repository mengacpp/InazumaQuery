#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "InazumaQuery/utils/list.h"
#include "InazumaQuery/utils/sort.h"

#define N 250

// Call once, e.g. at start of main()
static inline void init_rand(void)
{
    srand((unsigned)time(NULL));
}

// Returns a pseudo-random int in [0 .. max-1].
// Behavior is undefined if max <= 0.
static inline int rand_max(int max)
{
    return rand() % max;
}

int cmp(void const *e, void const *e2)
{
    return (*(int *)e) - (*(int *)e2);
}

void set(void *e, void const *v)
{
    *(int *)e = *(int *)v;
}

void print_int(FILE *f, void const *i_ptr)
{
    int i = *(int *)i_ptr;

    fprintf(f, "%i", i);
}

void sort(ina_list_t *ls, ina_list_cmp_fn_t compare_fn,
          ina_list_set_fn_t set_fn)
{
    printf("list start:\n");
    ina_list_fprint(ls, stdout, print_int, "\t");

    clock_t start = clock();
    ina_sort(ls, compare_fn, set_fn);
    clock_t end = clock();

    printf("\nlist result:\n");
    ina_list_fprint(ls, stdout, print_int, "\t");
    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTime taken to sort: %.6f seconds\n", cpu_secs);
};

int main()
{
    init_rand();

    ina_list_t *ls = ina_list_create(sizeof(int));

    for (int i = 0; i < N; ++i)
    {
        int j = rand_max(N);

        ina_list_add(ls, &j);
    }
    sort(ls, cmp, set);

    ina_list_destroy(&ls);
    return 0;
}