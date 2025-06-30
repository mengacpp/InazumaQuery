#include "inazuma/hmap.h"

#include <iostream>
int main()
{
    InaHmap *hmap = ina_hmap_create(10);

    if (!hmap)
    {
        std::cout << "[ERROR] hashmap is NULL\n";
        return 1;
    }

    ina_hmap_add(hmap, "key1", 10);
    ina_hmap_add(hmap, "key2", 11);

    {
        bool found;
        int val = ina_hmap_get(hmap, "key1", &found);

        if (!found) return 1;
    }
    {
        bool found;
        int val = ina_hmap_get(hmap, "key2", &found);

        if (!found) return 1;
    }


    ina_hmap_destroy(&hmap);

    return 0;
}