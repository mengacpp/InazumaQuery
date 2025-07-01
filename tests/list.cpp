#include <iostream>

#include "inazuma/core/errno.h"
#include "inazuma/utils/list.h"


int main()
{
    InaList *ls = ina_list_create(sizeof(int));


    if (!ls)
    {
        std::cerr << ina_strerrno(ina_errno) << "\n";
        return 1;
    }

    for (int i = 0; i < 10; ++i)
    {
        if (!ina_list_add(ls, &i))
        {
            std::cerr << ina_strerrno(ina_errno) << "\n";
            return 1;
        }
    }

    for (int i = 0; i < 10; ++i)
    {
        int *r = (int *)ina_list_at(ls, i);

        if (!r)
        {
            std::cerr << ina_strerrno(ina_errno) << "\n";
            return 1;
        }

        std::cout << *r << "\n";
    }


    ina_list_destroy(&ls);
    return 0;
}