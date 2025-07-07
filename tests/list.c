#include <stddef.h>
#include <stdio.h>

#include "InazumaQuery/core/errno.h"
#include "InazumaQuery/utils/list.h"

void print_int(FILE *f, void const *i_ptr)
{
    int i = *(int *)i_ptr;

    fprintf(f, "%i", i);
}

void print(void *e, size_t i, size_t count)
{
    printf("%i ", *(int *)e);
}

int main()
{
    ina_list_t *ls = ina_list_create(sizeof(int));

    if (!ls)
    {
        ina_perror("Failed to create list");
        return 1;
    }

    for (int i = 0; i < 10; ++i)
    {
        if (!ina_list_add(ls, &i))
        {
            ina_perror("Failed to add to list");
            ina_list_destroy(&ls);
            return 1;
        }
    }

    ina_list_foreach(ls, print);

    ina_list_destroy(&ls);
    return 0;
}