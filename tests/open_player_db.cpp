#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "inazuma/inazuma.h" // IWYU pragma: keep
#include "inazuma/player_db.h"

int main()
{
    std::cout << "[INFO] opening database\n";
    InaPlayerDB *db = ina_player_db_open(INA_DATA_PLAYER_IE3_DIR);

    if (!db)
    {
        std::cout << "[ERROR] database is null\n";
        ina_player_db_close(&db);
        return 1;
    }

    std::cout << "[INFO] closing database\n";
    ina_player_db_close(&db);

    return 0;
}