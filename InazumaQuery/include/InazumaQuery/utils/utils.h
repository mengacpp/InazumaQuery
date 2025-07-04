#ifndef INAZUMA_UTILS_UTILS_H_
#define INAZUMA_UTILS_UTILS_H_

#include "InazumaQuery/core/core.h"
#include <stddef.h>

INA_BEGIN_EXTERN_C

INA_API char *ina_read_file_content(char const *path_to_file);

// TODO improve names for these functions
INA_API void ina_normalise(char const *str, char *result, size_t len);

INA_API int ina_normalise_strcmp(char const *str1, char const *str2);

INA_API void ina_retrieve_digits(char const *str, char *result, size_t len);

INA_API int ina_string_to_int(char const *str, int base);

INA_END_EXTERN_C

#endif // INAZUMA_UTILS_UTILS_H_