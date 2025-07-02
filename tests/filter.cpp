#include "inazuma/utils/filter.h"
#include "inazuma/core/errno.h"
#include "inazuma/utils/list.h"
#include <ctime>
#include <iostream>
#include <ostream>
#include <stddef.h>

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 2500

static std::stringstream ss;

int extract(void *e)
{
    return *(int *)e;
}

bool rule(void const *e)
{
    int v = *(int *)e;
    return (v % 7 == 0);
}


std::string print_list(InaList const *ls)
{
    std::stringstream ss;

    ss << "[";

    for (size_t i = 0; i < ina_list_count(ls); ++i)
    {
        int r = extract(ina_list_at(ls, i));

        ss << r;

        if (i < ina_list_count(ls) - 1)
        {
            ss << ", ";
        }
    }

    ss << "]";

    return ss.str();
}


void filter(InaList *ls, InaFilterFn rule_fn)
{
    std::cout << "start:\n";
    std::cout << print_list(ls) << "\n" << std::flush;

    clock_t start = clock();
    InaList *result = ina_filter(ls, rule_fn);
    clock_t end = clock();

    if (!result)
    {
        ss << ina_strerrno(ina_errno) << "\n";
    }

    std::cout << "result:\n" << print_list(result) << "\n";
    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to sort: %.6f seconds\n", cpu_secs);

    ina_list_destroy(&result);

    std::cout << std::flush;
};

int main()
{
    InaList *ls = ina_list_create(sizeof(int));

    for (int i = 0; i < N; ++i)
    {
        if (!ina_list_add(ls, &i)) ss << ina_strerrno(ina_errno) << "\n";
    }

    filter(ls, rule);

    ina_list_destroy(&ls);

    std::cout << ss.str() << "\n";
    return 0;
}