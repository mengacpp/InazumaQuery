#include <cstdint>
#include <iostream>
#include <stddef.h>

#include "inazuma/inazuma.h" // IWYU pragma: keep
#include "inazuma/player.h"

int main()
{
    InaPlayerDB *ie3_db = ina_open_player_db(INA_DATA_PLAYER_IE3_DIR);

    if (!ie3_db)
    {
        std::cerr << "[TEST][ERROR] database is null\n";
        return 1;
    }

    uint16_t count = 0;
    InaPlayer *players = ina_get_players(ie3_db, 10, &count);

    for (size_t i = 0; i < count; ++i)
    {
        ina_player_print(&players[i]);
    }

    free(players);

    ina_close_player_db(&ie3_db);

    return 0;
}