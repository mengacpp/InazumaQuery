#ifndef INAZUMAQUERY_UTILS_UTILS_H_
#define INAZUMAQUERY_UTILS_UTILS_H_

#include "InazumaQuery/core/core.h"
#include <stddef.h>

INA_BEGIN_EXTERN_C

INA_API char *ina_file_readall(char const *path_to_file);

INA_API void ina_normalise_string(char const *str, char *result, size_t len);

INA_API int ina_strcmp_normalised(char const *str1, char const *str2);

INA_END_EXTERN_C

#endif // INAZUMAQUERY_UTILS_UTILS_H_