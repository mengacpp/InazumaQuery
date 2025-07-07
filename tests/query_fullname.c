#include "InazumaQuery/core/errno.h"
#include "InazumaQuery/inazuma.h" // IWYU pragma: keep

#include <stdio.h>
#include <time.h>

static char const *names[] = {"Mark Evans", "Adora Shivers", "Dee Tarrant",
                              "Sonny Wukong", "Kay Emcee"};

#define NAME_COUNT (sizeof(names) / sizeof(*name))

int main()
{
    bool error = false;
    ina_pdb_t *db = ina_pdb_create(INA_DEBUG_DATA_DIR "/players/ie3.csv");

    size_t arrayLength = sizeof names / sizeof names[0];
    clock_t start = clock();
    for (size_t i = 0; i < 1000; ++i)
    {
        char const *name = names[i % arrayLength];
        ina_player_t const *p = ina_pdb_find_fullname(db, name);

        if (!p)
        {
            fprintf(stderr, "Player %s not found: %s\n", name,
                    ina_strerrno(ina_errno));
            error = true;
            break;
        }
    }
    clock_t end = clock();

    ina_pdb_destroy(&db);

    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to find 1000 players: %.6f seconds\n", cpu_secs);

    return error ? 1 : 0;
}