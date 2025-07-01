#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "inazuma/inazuma.h" // IWYU pragma: keep

int main()
{
    std::cout << "[INFO] opening database\n";
    InaPlayerDB *db = ina_open_player_db(INA_DATA_PLAYER_IE3_DIR);

    if (!db)
    {
        std::cout << "[ERROR] database is null\n";
        ina_close_player_db(&db);
        return 1;
    }

    std::cout << "[INFO] closing database\n";
    ina_close_player_db(&db);

    return 0;
}