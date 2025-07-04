#include <stdio.h>

#include "InazumaQuery/utils/hash_map.h"

int main()
{
    ina_hash_map_t *hash_map = ina_hash_map_create(10);

    ina_hash_map_add(hash_map, "key1", 10);
    ina_hash_map_add(hash_map, "key2", 11);

    {
        bool found;
        int val = ina_hash_map_get(hash_map, "key1", &found);

        if (!found) return 1;
    }
    {
        bool found;
        int val = ina_hash_map_get(hash_map, "key2", &found);

        if (!found) return 1;
    }

    ina_hash_map_destroy(&hash_map);

    return 0;
}