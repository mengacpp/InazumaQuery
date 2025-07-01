#ifndef INAZUMA_UTILS_SORT_H_
#define INAZUMA_UTILS_SORT_H_

#include <stdbool.h>
#include <stddef.h>

#include "inazuma/core/core.h"

#include "inazuma/utils/list.h"


typedef int (*InaListElementKeyExtractorFn)(void *);
typedef void (*InaListElementKeySetterFn)(void *, int);


BEGIN_EXTERN_C

#define ina_sort(list, extract_key, set_key)                                   \
    __ina_sort(list, extract_key, set_key, 32, true);

INA_API void __ina_sort(InaList const *list,
                        InaListElementKeyExtractorFn extract_key_fn,
                        InaListElementKeySetterFn set_key_fn, int insertion_th,
                        bool heapsort);

END_EXTERN_C


#endif // INAZUMA_UTILS_SORT_H_