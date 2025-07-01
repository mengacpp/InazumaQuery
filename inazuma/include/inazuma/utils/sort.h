#ifndef INAZUMA_UTILS_SORT_H_
#define INAZUMA_UTILS_SORT_H_

#include <stdbool.h>
#include <stddef.h>

#include "inazuma/core/core.h"

#include "inazuma/utils/list.h"


typedef int (*InaListElementKeyExtractorFn)(void const *);
typedef void (*InaListElementKeySetterFn)(void const *, int);

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
                        InaListElementKeyExtractorFn extract_key_fn,
                        InaListElementKeySetterFn set_key_fn,
                        InaSortDir sort_dir, int insertion_th, bool heapsort);


INA_API void __ina_reverse(InaList const *list,
                           InaListElementKeyExtractorFn extract_key_fn,
                           InaListElementKeySetterFn set_key_fn);

INA_END_EXTERN_C


#endif // INAZUMA_UTILS_SORT_H_