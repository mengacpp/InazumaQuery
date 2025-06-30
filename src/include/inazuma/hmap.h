#ifndef INAZUMA_HASHMAP_H_
#define INAZUMA_HASHMAP_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "inazuma/core.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct InaHmap InaHmap;

    INA_API InaHmap *ina_hmap_create(size_t count);

    INA_API int ina_hmap_add(InaHmap *hmap, char const *key, uint16_t value);

    INA_API uint16_t ina_hmap_get(InaHmap *hmap, char const *key, bool *found);

    INA_API void ina_hmap_destroy(InaHmap **hmap);

#ifdef __cplusplus
}
#endif

#endif // INAZUMA_HASHMAP_H_