#ifndef INAZUMA_PLAYER_DB_H_
#define INAZUMA_PLAYER_DB_H_

#include <stdint.h>

#include "inazuma/core/core.h"

#include "inazuma/player.h"

#include "inazuma/utils/list.h"
#include "inazuma/utils/sort.h" // IWYU pragma: keep

typedef struct InaPlayerDB InaPlayerDB;

typedef int (*InaPlayerCompareFn)(InaPlayer const *, InaPlayer const *);
typedef bool (*InaPlayerFilterFn)(InaPlayer const *);


INA_BEGIN_EXTERN_C

INA_API bool ina_no_filtering(InaPlayer const *e);

int ina_no_sorting(InaPlayer const *e, InaPlayer const *e2);

INA_API InaPlayerDB *ina_player_db_open(char const *csv_path);

INA_API InaList *ina_player_db_get(InaPlayerDB const *db, uint16_t max_count,
                                   InaPlayerFilterFn filter_fn,
                                   InaPlayerCompareFn compare_fn);

#define ina_player_db_get_all(db)                                              \
    ina_player_db_get(db, 0, ina_no_filtering, ina_no_sorting);

#define ina_player_db_get_unrefined(db, count)                                 \
    ina_player_db_get(db, count, ina_no_filtering, ina_no_sorting);

#define ina_player_db_get_unsorted(db, count, filter_fn)                       \
    ina_player_db_get(db, count, filter_fn, ina_no_sorting);

#define ina_player_db_get_unfiltered(db, count, compare_fn)                    \
    ina_player_db_get(db, count, ina_no_filtering, compare_fn);

INA_API InaPlayer const *ina_player_db_get_by_fullname(InaPlayerDB const *db,
                                                       char const *fullname);

INA_API void ina_player_db_close(InaPlayerDB **db);

INA_END_EXTERN_C


#endif // INAZUMA_PLAYER_DB_H_