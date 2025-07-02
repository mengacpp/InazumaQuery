#ifndef INAZUMA_UTILS_SORT_H_
#define INAZUMA_UTILS_SORT_H_

#include <stdbool.h>
#include <stddef.h>

#include "inazuma/core/core.h"

#include "inazuma/utils/list.h"


typedef int (*InaListElementCompareFn)(void const *, void const *);
typedef void (*InaListElementSetFn)(void *, void const *);


INA_BEGIN_EXTERN_C

INA_API void ina_sort(InaList const *list, InaListElementCompareFn compare_fn,
                      InaListElementSetFn set_fn);


INA_API void ina_reverse(InaList const *list,
                         InaListElementCompareFn compare_fn,
                         InaListElementSetFn set_fn);

INA_END_EXTERN_C


#endif // INAZUMA_UTILS_SORT_H_