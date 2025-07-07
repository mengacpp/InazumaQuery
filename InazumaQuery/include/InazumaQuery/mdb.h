/*
    Moves DataBase
*/
#ifndef INAZUMAQUERY_MDB_H_
#define INAZUMAQUERY_MDB_H_

#include "InazumaQuery/core/core.h"

#include <stdbool.h>
#include <stddef.h>

#include "InazumaQuery/move.h"

#include "InazumaQuery/utils/list.h"

typedef struct ina_mdb_t ina_mdb_t;

typedef int (*ina_move_compare_fn_t)(ina_move_t const *, ina_move_t const *);
typedef bool (*ina_move_filter_fn_t)(ina_move_t const *);

INA_BEGIN_EXTERN_C

INA_API bool ina_mdb_accept_all(ina_move_t const *e);

INA_API int ina_mdb_default_compare(ina_move_t const *e, ina_move_t const *e2);

INA_API ina_mdb_t *ina_mdb_create(char const *csv_path);

INA_API ina_list_t *ina_mdb_query(ina_mdb_t const *db, uint16_t max_count,
                                  ina_move_filter_fn_t filter_fn,
                                  ina_move_compare_fn_t compare_fn);

INA_API ina_move_t const *ina_mdb_find_name(ina_mdb_t const *db,
                                            char const *name);

INA_API size_t ina_mdb_count(ina_mdb_t *db);

INA_API void ina_mdb_destroy(ina_mdb_t **db);

INA_END_EXTERN_C

#endif // INAZUMAQUERY_MDB_H_