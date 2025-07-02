#include "inazuma/utils/filter.h"
#include "inazuma/core/errno.h"
#include "inazuma/utils/list.h"
#include <stddef.h>
#include <stdio.h>

InaList *__ina_filter(InaList *ls, InaFilterFn filter_fn)
{
    if (!ls)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return NULL;
    }

    InaList *result = ina_list_create(ina_list_sizeof_element(ls));

    for (size_t i = 0; i < ina_list_count(ls); ++i)
    {
        fflush(stdout);

        void *e = ina_list_at(ls, i);

        if (filter_fn(e))
        {
            fflush(stdout);
            if (!ina_list_add(result, e))
            {
                ina_list_destroy(&result);
                return NULL;
            }
        }
    }

    return result;
}