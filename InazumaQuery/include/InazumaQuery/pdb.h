#ifndef INAZUMAQUERY_PDB_H_
#define INAZUMAQUERY_PDB_H_

#include <stdint.h>

#include "InazumaQuery/core/core.h"

#include "InazumaQuery/player.h"

#include "InazumaQuery/utils/list.h"
#include "InazumaQuery/utils/sort.h" // IWYU pragma: keep

typedef struct ina_pdb_t ina_pdb_t;

typedef int (*ina_player_compare_fn)(ina_player_t const *,
                                     ina_player_t const *);
typedef bool (*ina_player_filter_fn_t)(ina_player_t const *);


INA_BEGIN_EXTERN_C

INA_API bool ina_pdb_accept_all(ina_player_t const *e);

INA_API int ina_pdb_default_compare(ina_player_t const *e,
                                    ina_player_t const *e2);

INA_API ina_pdb_t *ina_pdb_create(char const *csv_path);

INA_API ina_list_t *ina_pdb_query(ina_pdb_t const *db, uint16_t max_count,
                                  ina_player_filter_fn_t filter_fn,
                                  ina_player_compare_fn compare_fn);

INA_API ina_player_t const *ina_pdb_find_fullname(ina_pdb_t const *db,
                                                  char const *fullname);

INA_API void ina_pdb_destroy(ina_pdb_t **db);

INA_END_EXTERN_C


#endif // INAZUMAQUERY_PDB_H_