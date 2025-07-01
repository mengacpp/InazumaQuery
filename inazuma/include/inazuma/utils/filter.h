#ifndef INAZUMA_UTILS_FILTER_H_
#define INAZUMA_UTILS_FILTER_H_

#include <stdbool.h>

#include "inazuma/core/core.h"
#include "inazuma/utils/list.h"

INA_BEGIN_EXTERN_C

typedef bool (*InaFilterRuleFn)(void const *);

#define ina_filter(ls, rule_fn) __ina_filter(ls, rule_fn)

INA_API InaList *__ina_filter(InaList *ls, InaFilterRuleFn rule_fn);

INA_END_EXTERN_C

#endif // INAZUMA_UTILS_FILTER_H_