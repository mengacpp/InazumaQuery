#ifndef INAZUMA_UTILS_LIST_H_
#define INAZUMA_UTILS_LIST_H_

#include "inazuma/core/core.h"
#include <stddef.h>

typedef struct InaList InaList;

INA_BEGIN_EXTERN_C

INA_API InaList *ina_list_create(size_t e_size);

INA_API int ina_list_add(InaList *ls, void *e);

INA_API void *ina_list_at(InaList const *ls, size_t id);

INA_API void ina_list_destroy(InaList **ls);

INA_API size_t ina_list_count(InaList const *ls);
INA_API size_t ina_list_sizeof_element(InaList const *ls);

INA_END_EXTERN_C

#endif // INAZUMA_UTILS_LIST_H_