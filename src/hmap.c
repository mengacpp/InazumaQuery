#include "inazuma/hmap.h"

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inazuma/core.h"
#include "inazuma/errno.h"

struct InaHmap
{
    char **keys;
    uint16_t *values;
    bool *used;
    size_t count;
};

unsigned int JSHash(const char *str, unsigned int length)
{
    unsigned int hash = 1315423911;
    unsigned int i = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }

    return hash;
}

InaHmap *ina_hmap_create(size_t count)
{
    InaHmap *hmap = malloc(sizeof(*hmap));
    if (!hmap)
    {
        ina_stderrno = errno;
        ina_errno = INA_ERRT_STDERROR;
        return NULL;
    }

    hmap->values = malloc(count * sizeof(*hmap->values));
    hmap->used = malloc(count * sizeof(*hmap->used));
    hmap->keys = malloc(count * sizeof(*hmap->keys));
    hmap->count = count;

    if (!hmap->values || !hmap->used)
    {
        ina_hmap_destroy(&hmap);
        ina_stderrno = errno;
        ina_errno = INA_ERRT_STDERROR;
        return NULL;
    }

    for (size_t i = 0; i < count; ++i)
    {
        hmap->used[i] = false;
    }

    return hmap;
}

int ina_hmap_add(InaHmap *hmap, char const *key, uint16_t value)
{
    unsigned int h = JSHash(key, strlen(key)) % hmap->count;

    unsigned int original_h = h;
    uint16_t i = 1;
    while (hmap->used[h])
    {
        if (strcmp(hmap->keys[h], key) == 0)
        {
            hmap->values[h] = value;
            return INA_SUCCESS;
        }

        h = (original_h + i * i) % hmap->count;
        i++;

        if (h == original_h)
        {
            ina_errno = INA_ERRT_HMAP_NOSPACE;
            return INA_FAILURE;
        }
    }

    hmap->keys[h] = strdup(key);
    hmap->used[h] = true;
    hmap->values[h] = value;

    return INA_SUCCESS;
}

uint16_t ina_hmap_get(InaHmap *hmap, char const *key, bool *found)
{
    unsigned int h = JSHash(key, strlen(key)) % hmap->count;

    unsigned int original_h = h;
    uint16_t i = 1;
    while (hmap->used[h])
    {
        if (strcmp(hmap->keys[h], key) == 0)
        {
            *found = true;
            return hmap->values[h];
        }

        h = (original_h + i * i) % hmap->count;
        i++;

        if (h == original_h)
        {
            break;
        }
    }

    *found = false;
    ina_errno = INA_ERRT_HMAP_NOTFOUND;
    return 0;
}

void ina_hmap_destroy(InaHmap **hmap)
{
    if (!(*hmap)) return;

    free((*hmap)->used);
    free((*hmap)->values);

    for (size_t i = 0; i < (*hmap)->count; ++i)
    {
        free((*hmap)->keys[i]);
    }

    *hmap = NULL;
    return;
}
