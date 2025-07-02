#include <stdio.h>

#include "inazuma/core/errno.h"
#include "inazuma/utils/list.h"

void print_int(FILE *f, void const *i_ptr)
{
    int i = *(int *)i_ptr;

    fprintf(f, "%i", i);
}

int main()
{
    ina_list_t *ls = ina_list_create(sizeof(int));

    for (int i = 0; i < 10; ++i)
    {
        if (!ina_list_add(ls, &i))
        {
            fprintf(stderr, "Failed to add to list: %s\n",
                    ina_strerrno(ina_errno));
            return 1;
        }
    }

    ina_list_fprint(ls, stdout, print_int, "\t");


    ina_list_destroy(&ls);
    return 0;
}