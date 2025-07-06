#ifndef INAZUMA_UTILS_LIST_H_
#define INAZUMA_UTILS_LIST_H_

#include "InazumaQuery/core/core.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct ina_list_t ina_list_t;

typedef void (*ina_list_elem_printer_t)(FILE *out, void const *elem);

INA_BEGIN_EXTERN_C

INA_API ina_list_t *ina_list_create(size_t e_size);

INA_API int ina_list_add(ina_list_t *ls, void *e);

INA_API void *ina_list_at(ina_list_t const *ls, size_t id);

INA_API void ina_list_destroy(ina_list_t **ls);

INA_API size_t ina_list_count(ina_list_t const *ls);

INA_API bool ina_list_copy(ina_list_t *dst, ina_list_t const *src,
                           size_t offset, size_t max_count);

INA_API size_t ina_list_sizeof_element(ina_list_t const *ls);

INA_API void ina_list_fprint(ina_list_t const *ls, FILE *out,
                             ina_list_elem_printer_t elem_printer,
                             char const *sep);

INA_END_EXTERN_C

#endif // INAZUMA_UTILS_LIST_H_