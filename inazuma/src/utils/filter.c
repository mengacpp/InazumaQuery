#include "inazuma/utils/filter.h"

#include <stddef.h>
#include <stdio.h>

#include "inazuma/core/errno.h"
#include "inazuma/utils/list.h"

ina_list_t *ina_filter(ina_list_t *ls, ina_filter_fn_t filter_fn)
{
    if (!ls)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return NULL;
    }

    ina_list_t *result = ina_list_create(ina_list_sizeof_element(ls));

    for (size_t i = 0; i < ina_list_count(ls); ++i)
    {
        void *e = ina_list_at(ls, i);

        if (filter_fn(e))
        {
            if (!ina_list_add(result, e))
            {
                ina_list_destroy(&result);
                return NULL;
            }
        }
    }

    return result;
}