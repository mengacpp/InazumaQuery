#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "inazuma/core.h"
#include "inazuma/player.h"

typedef struct InaPlayerDB InaPlayerDB;

#ifdef __cplusplus
extern "C"
{
#endif

    INA_API InaPlayerDB *ina_open_player_db(char const *csv_path);

    INA_API InaPlayer *ina_get_players(InaPlayerDB const *db, size_t max_count,
                                       size_t *count);

    INA_API InaPlayer const *ina_get_player_by_fullname(InaPlayerDB const *db,
                                                        char const *fullname);

    INA_API void ina_close_player_db(InaPlayerDB **db);

#ifdef __cplusplus
}
#endif