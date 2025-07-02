#include "inazuma/inazuma.h" // IWYU pragma: keep
#include "inazuma/player.h"
#include "inazuma/utils/list.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// take a look at
// https://github.com/mengacpp/inazuma-api/blob/main/examples/query.c to
// understand how querying works first

// we use this simple function to calcualte if a has alphabetical priority over
// b
int compare(ina_player_t const *a, ina_player_t const *b)
{
    size_t len = strlen(a->fullname_normalised) > strlen(b->fullname_normalised)
                     ? strlen(b->fullname_normalised)
                     : strlen(a->fullname_normalised);

    for (size_t i = 0; i < len; ++i)
    {
        uint8_t ca = tolower(a->fullname_normalised[i]);
        uint8_t cb = tolower(b->fullname_normalised[i]);

        if (ca == cb) continue;

        return ca - cb;
    };

    return 0;
}

bool filter(ina_player_t const *p)
{
    return true;
}

void print_name(FILE *f, void const *e)
{
    ina_player_t *p = (ina_player_t *)e;

    fprintf(f, "%s", p->fullname);
}

int main()
{
    ina_db_t *ie3_db = ina_db_create_from_csv(INA_IE3_CSV);


    clock_t start = clock();
    ina_list_t *players = ina_db_query(ie3_db, 100, filter, compare);
    clock_t end = clock();

    ina_list_fprint(players, stdout, print_name, "\n");

    ina_list_destroy(&players);

    ina_db_close(&ie3_db);

    return 0;
}