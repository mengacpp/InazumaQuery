#ifndef INAZUMAQUERY_UTILS_FILTER_H_
#define INAZUMAQUERY_UTILS_FILTER_H_

#include <stdbool.h>

#include "InazumaQuery/core/core.h"
#include "InazumaQuery/utils/list.h"

INA_BEGIN_EXTERN_C

typedef bool (*ina_filter_fn_t)(void const *);

INA_API ina_list_t *ina_filter(ina_list_t *ls, ina_filter_fn_t filter_fn);

INA_END_EXTERN_C

#endif // INAZUMAQUERY_UTILS_FILTER_H_