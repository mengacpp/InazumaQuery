#include "inazuma/core.h"
#include "inazuma/errno.h"
#include "inazuma/inazuma.h"
#include "inazuma/player.h"

#include <iostream>
#include <string>

std::string names[] = {"Mark Evans", "Adora Shivers", "Dee Tarrant",
                       "Sonny Wukong", "Kay Emcee"};

int main()
{
    bool error = false;
    InaPlayerDB *db = ina_open_player_db(INA_DATA_PLAYER_IE3_DIR);

    if (!db)
    {
        std::cout << "Error when reading db '" << INA_DATA_PLAYER_IE3_DIR
                  << "': " << ina_strerrno(ina_errno);
        return 1;
    }

    for (auto name : names)
    {
        InaPlayer *p = ina_get_player_by_fullname(db, name.c_str());

        if (!p)
        {
            std::cerr << "player " << name
                      << " not found: " << ina_strerrno(ina_errno) << "\n";
            error = true;
            break;
        }
    }

    ina_close_player_db(&db);

    return error ? 1 : 0;
}