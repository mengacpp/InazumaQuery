#ifndef INAZUMA_UTILS_HMAP_H_
#define INAZUMA_UTILS_HMAP_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "inazuma/core/core.h"


typedef struct InaHmap InaHmap;

BEGIN_EXTERN_C

INA_API InaHmap *ina_hmap_create(size_t count);

INA_API int ina_hmap_add(InaHmap *hmap, char const *key, uint16_t value);

INA_API uint16_t ina_hmap_get(InaHmap *hmap, char const *key, bool *found);

INA_API void ina_hmap_destroy(InaHmap **hmap);

END_EXTERN_C

#endif // INAZUMA_UTILS_HMAP_H_