#ifndef INAZUMAQUERY_UTILS_SORT_H_
#define INAZUMAQUERY_UTILS_SORT_H_

#include <stdbool.h>
#include <stddef.h>

#include "InazumaQuery/core/core.h"

#include "InazumaQuery/utils/list.h"


typedef int (*ina_list_cmp_fn_t)(void const *a, void const *b);
typedef void (*ina_list_set_fn_t)(void *dst, void const *src);


INA_BEGIN_EXTERN_C

INA_API void ina_sort(ina_list_t const *list, ina_list_cmp_fn_t compare_fn,
                      ina_list_set_fn_t set_fn);

INA_END_EXTERN_C


#endif // INAZUMAQUERY_UTILS_SORT_H_