#ifndef INAZUMA_UTILS_SORT_H_
#define INAZUMA_UTILS_SORT_H_

#include <stdbool.h>
#include <stddef.h>

#include "inazuma/core/core.h"

#include "inazuma/utils/list.h"


typedef int (*InaListElementGetComparatorFn)(void const *);
typedef void (*InaListElementSetFn)(void *, void const *);

typedef enum InaSortDir
{
    INA_SORT_DIR_ASCENDING,
    INA_SORT_DIR_DESCENDING,
} InaSortDir;


INA_BEGIN_EXTERN_C

#define ina_sort(list, extract_key_fn, set_key_fn, sort_dir)                   \
    __ina_sort(list, extract_key_fn, set_key_fn, sort_dir, 32, true);


#define ina_reverse(list, extract_key_fn, set_key_fn)                          \
    __ina_reverse(list, extract_key_fn, set_key_fn)

INA_API void __ina_sort(InaList const *list,
                        InaListElementGetComparatorFn extract_key_fn,
                        InaListElementSetFn set_key_fn, InaSortDir sort_dir,
                        int insertion_th, bool heapsort);


INA_API void __ina_reverse(InaList const *list,
                           InaListElementGetComparatorFn extract_key_fn,
                           InaListElementSetFn set_key_fn);

INA_END_EXTERN_C


#endif // INAZUMA_UTILS_SORT_H_