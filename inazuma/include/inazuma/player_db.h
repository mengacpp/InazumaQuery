#ifndef INAZUMA_PLAYER_DB_H_
#define INAZUMA_PLAYER_DB_H_

#include "inazuma/core/core.h"

#include "inazuma/player.h"
#include <stdint.h>

typedef struct InaPlayerDB InaPlayerDB;

typedef struct InaPlayerDBSort
{
    InaPlayerAttributeType target_attribute;
} InaPlayerDBSort;

typedef struct InaPlayerDBFilter
{
} InaPlayerDBFilter;

// SORT:
// attribute
// ascending / descending

// FILTER
// attribute
// Equal / Greater / Less
// Treshold

BEGIN_EXTERN_C

INA_API InaPlayerDB *ina_open_player_db(char const *csv_path);

INA_API InaPlayer *ina_get_players(InaPlayerDB const *db, uint16_t max_count,
                                   uint16_t *count);

INA_API InaPlayer const *ina_get_player_by_fullname(InaPlayerDB const *db,
                                                    char const *fullname);

INA_API void ina_close_player_db(InaPlayerDB **db);

END_EXTERN_C


#endif // INAZUMA_PLAYER_DB_H_