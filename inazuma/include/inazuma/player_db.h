#ifndef INAZUMA_PLAYER_DB_H_
#define INAZUMA_PLAYER_DB_H_

#include <stdint.h>

#include "inazuma/core/core.h"

#include "inazuma/player.h"

#include "inazuma/utils/list.h"
#include "inazuma/utils/sort.h"

typedef struct InaPlayerDB InaPlayerDB;

typedef struct InaPlayerDBSort
{
    InaPlayerAttribute target_attribute;
    InaSortDir direction;

} InaPlayerDBSort;

typedef struct InaPlayerDBFilter
{
    InaPlayerAttribute target_attribute;
    uint8_t treshold;

} InaPlayerDBFilter;

// SORT:
// attribute
// ascending / descending

// FILTER
// attribute
// Equal / Greater / Less
// Treshold

INA_BEGIN_EXTERN_C

INA_API InaPlayerDB *ina_player_db_open(char const *csv_path);

INA_API InaList *ina_player_db_get(InaPlayerDB const *db, uint16_t max_count);

INA_API InaPlayer const *ina_player_db_get_by_fullname(InaPlayerDB const *db,
                                                       char const *fullname);

INA_API void ina_player_db_close(InaPlayerDB **db);

INA_END_EXTERN_C


#endif // INAZUMA_PLAYER_DB_H_