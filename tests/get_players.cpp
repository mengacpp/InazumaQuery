#include <iostream>
#include <stddef.h>

#include "inazuma/core/errno.h"
#include "inazuma/inazuma.h" // IWYU pragma: keep
#include "inazuma/player.h"
#include "inazuma/player_db.h"
#include "inazuma/utils/list.h"
#include "inazuma/utils/sort.h"

int get_comparator(InaPlayer const *p)
{
    return p->lvl99_tp;
}

int main()
{
    InaPlayerDB *ie3_db = ina_player_db_open(INA_DATA_PLAYER_IE3_DIR);

    if (!ie3_db)
    {
        std::cerr << "[TEST][ERROR] database is null\n";
        return 1;
    }
    clock_t start = clock();
    InaList *players = ina_player_db_get_unfiltered(ie3_db, 10, get_comparator,
                                                    INA_SORT_DIR_DESCENDING);
    clock_t end = clock();

    if (!players)
    {
        std::cerr << ina_strerrno(ina_errno) << "\n";
    }

    for (size_t i = 0; i < ina_list_count(players); ++i)
    {
        ina_player_print((InaPlayer *)ina_list_at(players, i));
    }
    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to filter and sort %zu players: %.6f seconds\n",
           ina_list_count(players), cpu_secs);

    ina_list_destroy(&players);

    ina_player_db_close(&ie3_db);

    return 0;
}