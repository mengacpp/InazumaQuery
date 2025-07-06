#ifndef INAZUMAQUERY_UTILS_STRING_H_
#define INAZUMAQUERY_UTILS_STRING_H_

#include "InazumaQuery/core/core.h"

#include <string.h>

typedef struct ina_string_t ina_string_t;

INA_BEGIN_EXTERN_C

INA_API ina_string_t *ina_string_create(char const *val);

INA_API int ina_string_set(ina_string_t *str, char const *val);

INA_API int ina_string_append(ina_string_t *str, char const *val);

INA_API char const *ina_string_cstr(ina_string_t *str);

INA_API int ina_string_cmp(ina_string_t *a, ina_string_t *b);

INA_API size_t ina_string_len(ina_string_t *a);

INA_API void ina_string_destroy(ina_string_t **str);

INA_END_EXTERN_C


#endif // INAZUMAQUERY_UTILS_STRING_H_