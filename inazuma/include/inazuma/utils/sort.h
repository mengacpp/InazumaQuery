#ifndef INAZUMA_UTILS_SORT_H_
#define INAZUMA_UTILS_SORT_H_

#include <stdbool.h>
#include <stddef.h>

#include "inazuma/core/core.h"


BEGIN_EXTERN_C

#define ina_sort(arr, len) _ina_sort(arr, len, 32, true);

INA_API void __ina_sort(int arr[], int len, int insertion_th, bool heapsort);

END_EXTERN_C


#endif // INAZUMA_UTILS_SORT_H_