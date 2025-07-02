#ifndef INAZUMA_UTILS_FILTER_H_
#define INAZUMA_UTILS_FILTER_H_

#include <stdbool.h>

#include "inazuma/core/core.h"
#include "inazuma/utils/list.h"

INA_BEGIN_EXTERN_C

typedef bool (*ina_filter_fn_t)(void const *);

INA_API ina_list_t *ina_filter(ina_list_t *ls, ina_filter_fn_t filter_fn);

INA_END_EXTERN_C

#endif // INAZUMA_UTILS_FILTER_H_