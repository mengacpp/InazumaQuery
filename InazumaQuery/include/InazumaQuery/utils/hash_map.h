#ifndef INAZUMA_UTILS_HMAP_H_
#define INAZUMA_UTILS_HMAP_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "InazumaQuery/core/core.h"

// TODO make hash map work with ina_list_t abstraction as well.
// already did this with sorting and filtering algorithms, should do it
// here for consistency and it may be useful in the future


typedef struct ina_hash_map_t ina_hash_map_t;

INA_BEGIN_EXTERN_C

INA_API ina_hash_map_t *ina_hash_map_create(size_t count);

INA_API int ina_hash_map_add(ina_hash_map_t *hmap, char const *key,
                             uint16_t value);

INA_API uint16_t ina_hash_map_get(ina_hash_map_t *hmap, char const *key,
                                  bool *found);

INA_API void ina_hash_map_destroy(ina_hash_map_t **hmap);

INA_END_EXTERN_C

#endif // INAZUMA_UTILS_HMAP_H_