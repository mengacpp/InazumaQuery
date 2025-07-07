#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "InazumaQuery/core/errno.h"
#include "InazumaQuery/utils/filter.h"
#include "InazumaQuery/utils/list.h"


#define N 2500

int extract(void *e)
{
    return *(int *)e;
}

bool rule(void const *e)
{
    int v = *(int *)e;
    return (v % 7 == 0);
}

void print_int(void *i_ptr, size_t i, size_t count)
{
    int val = *(int *)i_ptr;

    fprintf(stdout, "%i ", val);
}

void filter(ina_list_t *ls, ina_filter_fn_t rule_fn)
{
    printf("start list:\n");
    ina_list_foreach(ls, print_int);

    clock_t start = clock();
    ina_list_t *result = ina_filter(ls, rule_fn);
    clock_t end = clock();

    if (!result)
    {
        ina_perror("Failed to filter");
    }

    printf("\nresult list:\n");
    ina_list_foreach(result, print_int);

    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTime taken to sort: %.6f seconds\n", cpu_secs);

    ina_list_destroy(&result);
};

int main()
{
    ina_list_t *ls = ina_list_create(sizeof(int));

    for (int i = 0; i < N; ++i)
    {
        ina_list_add(ls, &i);
    }

    filter(ls, rule);

    ina_list_destroy(&ls);

    return 0;
}