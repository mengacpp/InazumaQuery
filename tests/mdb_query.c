#include "InazumaQuery/core/errno.h"
#include "InazumaQuery/element.h"
#include "InazumaQuery/inazuma.h" // IWYU pragma: keep
#include "InazumaQuery/mdb.h"
#include "InazumaQuery/move.h"
#include "InazumaQuery/utils/list.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int compare(ina_move_t const *a, ina_move_t const *b)
{
    size_t len = strlen(a->name_normalised) > strlen(b->name_normalised)
                     ? strlen(b->name_normalised)
                     : strlen(a->name_normalised);

    for (size_t i = 0; i < len; ++i)
    {
        int8_t ca = tolower(a->name_normalised[i]);
        int8_t cb = tolower(b->name_normalised[i]);

        if (ca == cb) continue;

        return ca - cb;
    };

    return 0;
}

bool filter(ina_move_t const *m)
{
    return m->element == INA_ELEMENT_WIND;
}

void print(void *e, size_t i, size_t count)
{
    ina_move_t *m = e;

    ina_move_fprint(stdout, m);
    printf("\n");
}

int main()
{
    ina_mdb_t *ie2_moves = ina_mdb_create(INA_DEBUG_DATA_DIR "/moves/ie2.csv");

    if (!ie2_moves)
    {
        ina_perror("Database not open");
        return EXIT_FAILURE;
    }

    printf("Found %zu moves in database.\n", ina_mdb_count(ie2_moves));

    clock_t start = clock();
    ina_list_t *moves = ina_mdb_query(ie2_moves, 100, filter, compare);
    clock_t end = clock();


    if (!moves)
    {
        ina_perror("Moves not found");
        ina_mdb_destroy(&ie2_moves);
        return EXIT_FAILURE;
    }

    ina_list_foreach(moves, print);

    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTime taken to filter and sort %zu moves: %.6f seconds\n",
           ina_list_count(moves), cpu_secs);

    ina_list_destroy(&moves);
    ina_mdb_destroy(&ie2_moves);

    return 0;
}