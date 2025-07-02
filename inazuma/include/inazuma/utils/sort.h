#ifndef INAZUMA_UTILS_SORT_H_
#define INAZUMA_UTILS_SORT_H_

#include <stdbool.h>
#include <stddef.h>

#include "inazuma/core/core.h"

#include "inazuma/utils/list.h"


typedef int (*ina_list_cmp_fn_t)(void const *a, void const *b);
typedef void (*ina_list_set_fn_t)(void *dst, void const *src);


INA_BEGIN_EXTERN_C

INA_API void ina_sort(ina_list_t const *list, ina_list_cmp_fn_t compare_fn,
                      ina_list_set_fn_t set_fn);


// TODO delete this function, it's not used anymore
INA_API void ina_reverse(ina_list_t const *list, ina_list_set_fn_t set_fn);

INA_END_EXTERN_C


#endif // INAZUMA_UTILS_SORT_H_