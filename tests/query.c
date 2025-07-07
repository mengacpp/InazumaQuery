#include "InazumaQuery/inazuma.h" // IWYU pragma: keep
#include "InazumaQuery/player.h"
#include "InazumaQuery/utils/list.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int compare(ina_player_t const *a, ina_player_t const *b)
{
    size_t len = strlen(a->fullname_normalised) > strlen(b->fullname_normalised)
                     ? strlen(b->fullname_normalised)
                     : strlen(a->fullname_normalised);

    for (size_t i = 0; i < len; ++i)
    {
        int8_t ca = tolower(a->fullname_normalised[i]);
        int8_t cb = tolower(b->fullname_normalised[i]);

        if (ca == cb) continue;

        return ca - cb;
    };

    return 0;
}

bool filter(ina_player_t const *p)
{
    return p->position == PLAYER_POSITION_DF &&
           p->fullname_normalised[0] == 'a';
}

void print_name(FILE *f, void const *e)
{
    ina_player_t *p = (ina_player_t *)e;

    fprintf(f, "%s", p->fullname);
}

int main()
{
    ina_pdb_t *ie3_db = ina_pdb_create(INA_DEBUG_DATA_DIR "/players/ie3.csv");


    clock_t start = clock();
    ina_list_t *players = ina_pdb_query(ie3_db, 100, filter, compare);
    clock_t end = clock();

    ina_list_fprint(players, stdout, print_name, "\n");

    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTime taken to filter and sort %zu players: %.6f seconds\n",
           ina_list_count(players), cpu_secs);

    ina_list_destroy(&players);

    ina_pdb_destroy(&ie3_db);

    return 0;
}