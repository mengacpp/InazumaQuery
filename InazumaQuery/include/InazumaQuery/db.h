#ifndef INAZUMA_PLAYER_DB_H_
#define INAZUMA_PLAYER_DB_H_

#include <stdint.h>

#include "InazumaQuery/core/core.h"

#include "InazumaQuery/player.h"

#include "InazumaQuery/utils/list.h"
#include "InazumaQuery/utils/sort.h" // IWYU pragma: keep

typedef struct ina_db_t ina_db_t;

typedef int (*ina_player_compare_fn)(ina_player_t const *,
                                     ina_player_t const *);
typedef bool (*ina_player_filter_fn_t)(ina_player_t const *);


INA_BEGIN_EXTERN_C

INA_API bool ina_db_accept_all(ina_player_t const *e);

int ina_db_default_compare(ina_player_t const *e, ina_player_t const *e2);

INA_API ina_db_t *ina_db_create_from_csv(char const *csv_path);

INA_API ina_list_t *ina_db_query(ina_db_t const *db, uint16_t max_count,
                                 ina_player_filter_fn_t filter_fn,
                                 ina_player_compare_fn compare_fn);

#define ina_db_query_all(db)                                                   \
    ina_db_query(db, 0, ina_db_accept_all, ina_db_default_compare);

INA_API ina_player_t const *ina_db_query_fullname(ina_db_t const *db,
                                                  char const *fullname);

INA_API void ina_db_close(ina_db_t **db);

INA_END_EXTERN_C


#endif // INAZUMA_PLAYER_DB_H_