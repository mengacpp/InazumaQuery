#include "inazuma/utils/sort.h"
#include "inazuma/utils/list.h"
#include <ctime>
#include <iostream>
#include <ostream>
#include <stddef.h>

#include <sstream>
#include <stdlib.h>
#include <time.h>

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

int compare(void const *e, void const *e2)
{
    return (*(int *)e) - (*(int *)e2);
}

void set(void *e, void const *v)
{
    *(int *)e = *(int *)v;
}

std::string print_list(InaList *ls)
{
    std::stringstream ss;

    ss << "[";

    for (size_t i = 0; i < ina_list_count(ls); ++i)
    {
        int r = *(int *)ina_list_at(ls, i);

        ss << r;

        if (i < ina_list_count(ls) - 1)
        {
            ss << ", ";
        }
    }

    ss << "]";

    return ss.str();
}


void sort(InaList *ls, InaListElementCompareFn compare_fn,
          InaListElementSetFn set_fn)
{
    std::cout << "start:\n";
    std::cout << print_list(ls) << "\n" << std::flush;

    clock_t start = clock();
    ina_sort(ls, compare_fn, set_fn);
    clock_t end = clock();

    std::cout << "result:\n" << print_list(ls) << "\n";
    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to sort: %.6f seconds\n", cpu_secs);

    std::cout << std::flush;
};

int main()
{
    init_rand();

    InaList *ls = ina_list_create(sizeof(int));

    for (int i = 0; i < N; ++i)
    {
        int j = rand_max(N);

        ina_list_add(ls, &j);
    }
    sort(ls, compare, set);

    ina_list_destroy(&ls);
    return 0;
}