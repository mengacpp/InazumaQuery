#include "inazuma/core.h"
#include "inazuma/errno.h"
#include "inazuma/inazuma.h"
#include "inazuma/player.h"

#include <cstddef>
#include <ctime>
#include <iostream>
#include <string>
#include <time.h>

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

    size_t arrayLength = sizeof names / sizeof names[0];
    clock_t start = clock();
    for (size_t i = 0; i < 1000; ++i)
    {
        char const *name = names[i % arrayLength].c_str();
        InaPlayer const *p = ina_get_player_by_fullname(db, name);

        if (!p)
        {
            std::cerr << "player " << name
                      << " not found: " << ina_strerrno(ina_errno) << "\n";
            error = true;
            break;
        }
    }
    clock_t end = clock();

    ina_close_player_db(&db);

    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to find 1000 players: %.6f seconds\n", cpu_secs);

    return error ? 1 : 0;
}