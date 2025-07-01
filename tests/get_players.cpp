#include <iostream>
#include <stddef.h>

#include "inazuma/inazuma.h" // IWYU pragma: keep
#include "inazuma/player.h"
#include "inazuma/player_db.h"
#include "inazuma/utils/list.h"

int main()
{
    InaPlayerDB *ie3_db = ina_player_db_open(INA_DATA_PLAYER_IE3_DIR);

    if (!ie3_db)
    {
        std::cerr << "[TEST][ERROR] database is null\n";
        return 1;
    }

    InaList *players = ina_player_db_get(ie3_db, 10);

    for (size_t i = 0; i < ina_list_count(players); ++i)
    {
        ina_player_print((InaPlayer *)ina_list_at(players, i));
    }

    ina_list_destroy(&players);

    ina_player_db_close(&ie3_db);

    return 0;
}