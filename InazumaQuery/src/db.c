#include "InazumaQuery/db.h"

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "InazumaQuery/core/errno.h"

#include "InazumaQuery/player.h"

#include "InazumaQuery/utils/csv.h"
#include "InazumaQuery/utils/filter.h"
#include "InazumaQuery/utils/hash_map.h"
#include "InazumaQuery/utils/list.h"
#include "InazumaQuery/utils/sort.h"
#include "InazumaQuery/utils/utils.h"

#define INA_PLAYER_DB_MAX_PLAYERS 2500

bool ina_db_accept_all(ina_player_t const *e)
{
    return true;
};

int ina_db_default_compare(ina_player_t const *e, ina_player_t const *e2)
{
    return e->hex_id - e2->hex_id;
};


struct ina_db_t
{
    ina_list_t *players;
    ina_hash_map_t *fullname_hmap;
};

int assign_to_player_by_col(ina_player_t *player, size_t col,
                            char const *cell_content);
int get_stat_from_cell(char const *cell_content);

ina_db_t *ina_db_create_from_csv(const char *csv_path)
{
    ina_db_t *db = malloc(sizeof(*db));
    if (!db)
    {
        ina_stderrno = errno;
        ina_errno = INA_ERRT_STD;
        return NULL;
    }

    db->players = ina_list_create(sizeof(ina_player_t));
    if (!db->players)
    {
        ina_db_close(&db);
        return NULL;
    }

    db->fullname_hmap = ina_hash_map_create(INA_PLAYER_DB_MAX_PLAYERS);

    if (!db->fullname_hmap)
    {
        ina_db_close(&db);
        return NULL;
    }

    ina_csv_t *csv = ina_csv_create(csv_path);

    if (!csv)
    {
        ina_db_close(&db);
        return NULL;
    }

    for (size_t row = 0; row < ina_csv_row_count(csv); ++row)
    {
        ina_player_t p;
        for (size_t col = 0; col < 45; ++col)
        {
            char const *cell = ina_csv_get_cell(csv, row, col);

            assign_to_player_by_col(&p, col, cell);
        }

        ina_list_add(db->players, &p);
        ina_hash_map_add(db->fullname_hmap, p.fullname_normalised,
                         ina_list_count(db->players) - 1);
    }

    ina_csv_destroy(&csv);

    return db;
}

void ina_db_close(ina_db_t **db)
{
    if (!(*db)) return;

    if ((*db)->players) ina_list_destroy(&(*db)->players);
    if ((*db)->fullname_hmap) ina_hash_map_destroy(&(*db)->fullname_hmap);

    free(*db);

    *db = NULL;
}

static ina_player_filter_fn_t g_filter_fn;
static ina_player_compare_fn g_compare_fn;

bool filter_wrapper(void const *e)
{
    return g_filter_fn((void const *)e);
}

int compare_wrapper(void const *e, void const *e2)
{
    return g_compare_fn((ina_player_t const *)e, (ina_player_t const *)e2);
}

void set(void *e, void const *e2)
{
    ina_player_t *player = e;

    ina_player_t const *player2 = e2;

    memcpy(player, player2, sizeof(ina_player_t));
}

// TODO return a list of pointers instead of a list of players
// this will avoid copying all that memory
INA_API ina_list_t *ina_db_query(ina_db_t const *db, uint16_t max_count,
                                 ina_player_filter_fn_t filter_fn,
                                 ina_player_compare_fn compare_fn)
{
    if (!db)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return NULL;
    }

    g_compare_fn = compare_fn ? compare_fn : ina_db_default_compare;
    g_filter_fn = filter_fn ? filter_fn : ina_db_accept_all;

    ina_list_t *filtered = ina_filter(db->players, filter_wrapper);
    ina_sort(filtered, compare_wrapper, set);

    if (max_count == 0) max_count = ina_list_count(db->players);
    size_t count = max_count > ina_list_count(filtered)
                       ? ina_list_count(filtered)
                       : max_count;

    ina_list_t *result = ina_list_create(sizeof(ina_player_t));
    if (!ina_list_copy(result, filtered, 0, count))
    {
        ina_list_destroy(&filtered);
        ina_list_destroy(&result);
        return NULL;
    }

    ina_list_destroy(&filtered);

    return result;
}

ina_player_t const *ina_db_query_fullname(ina_db_t const *db,
                                          char const *fullname)
{
    char fullname_normalised[INA_FULLNAME_MAX_LEN];

    ina_normalise_string(fullname, fullname_normalised, INA_FULLNAME_MAX_LEN);

    bool found;
    unsigned int id =
        ina_hash_map_get(db->fullname_hmap, fullname_normalised, &found);

    if (!found) return NULL;

    return ina_list_at(db->players, id);
}

#define ASSIGN_AND_BREAK(stat)                                                 \
    do                                                                         \
    {                                                                          \
        player->stat = strtol(cell_content, NULL, 10);                         \
        break;                                                                 \
    } while (0)

int assign_to_player_by_col(ina_player_t *player, size_t col,
                            char const *cell_content)
{
    switch (col)
    {
    case 0:
    {
        strncpy(player->fullname, cell_content, INA_FULLNAME_MAX_LEN);

        ina_normalise_string(player->fullname, player->fullname_normalised,
                             INA_FULLNAME_MAX_LEN);

        break;
    }
    case 1:
    {
        strncpy(player->nickname, cell_content, INA_FULLNAME_MAX_LEN);

        ina_normalise_string(player->nickname, player->nickname_normalised,
                             INA_FULLNAME_MAX_LEN);
        break;
    }
    case 2:
    {
        int is_gk = ina_strcmp_normalised("GK", cell_content) == 0;
        int is_df = ina_strcmp_normalised("DF", cell_content) == 0;
        int is_mf = ina_strcmp_normalised("MF", cell_content) == 0;
        int is_fw = ina_strcmp_normalised("FW", cell_content) == 0;

        if (is_gk)
            player->position = PLAYER_POSITION_GK;
        else if (is_df)
            player->position = PLAYER_POSITION_DF;
        else if (is_mf)
            player->position = PLAYER_POSITION_MF;
        else if (is_fw)
            player->position = PLAYER_POSITION_FW;
        else
        {
            fprintf(stderr, "ERROR: Invalid player position '%s'\n",
                    cell_content);
            return 0;
        }

        break;
    }
    case 3:
    {
        int is_male = ina_strcmp_normalised("male", cell_content) == 0;
        int is_female = ina_strcmp_normalised("female", cell_content) == 0;

        if (is_male)
            player->gender = PLAYER_GENDER_MALE;
        else if (is_female)
            player->gender = PLAYER_GENDER_FEMALE;
        else
        {
            ina_errno = INA_ERRT_CSV_CELL_INVALID;
            return 0;
        }

        break;
    }
    case 4:
    {
        int is_small = ina_strcmp_normalised("small", cell_content) == 0;
        int is_medium = ina_strcmp_normalised("medium", cell_content) == 0;
        int is_large = ina_strcmp_normalised("large", cell_content) == 0;

        if (is_small)
            player->size = PLAYER_SIZE_SMALL;
        else if (is_medium)
            player->size = PLAYER_SIZE_MEDIUM;
        else if (is_large)
            player->size = PLAYER_SIZE_LARGE;
        else
        {
            ina_errno = INA_ERRT_CSV_CELL_INVALID;
            return 0;
        }

        break;
    }
    case 5:
    {
        int is_air = ina_strcmp_normalised("air", cell_content) == 0;
        int is_earth = ina_strcmp_normalised("earth", cell_content) == 0;
        int is_fire = ina_strcmp_normalised("fire", cell_content) == 0;
        int is_wood = ina_strcmp_normalised("wood", cell_content) == 0;

        if (is_air)
            player->element = ELEMENT_AIR;
        else if (is_earth)
            player->element = ELEMENT_EARTH;
        else if (is_fire)
            player->element = ELEMENT_FIRE;
        else if (is_wood)
            player->element = ELEMENT_WOOD;
        else
        {
            ina_errno = INA_ERRT_CSV_CELL_INVALID;
            return 0;
        }

        break;
    }
    case 6:
    {
        ASSIGN_AND_BREAK(lvl1_fp);
    }
    case 7:
    {
        ASSIGN_AND_BREAK(lvl1_tp);
    }
    case 8:
    {
        ASSIGN_AND_BREAK(lvl1_kick);
    }
    case 9:
    {
        ASSIGN_AND_BREAK(lvl1_body);
    }
    case 10:
    {
        ASSIGN_AND_BREAK(lvl1_control);
    }
    case 11:
    {
        ASSIGN_AND_BREAK(lvl1_guard);
    }
    case 12:
    {
        ASSIGN_AND_BREAK(lvl1_speed);
    }
    case 13:
    {
        ASSIGN_AND_BREAK(lvl1_stamina);
    }
    case 14:
    {
        ASSIGN_AND_BREAK(lvl1_guts);
    }
    case 15:
    {
        ASSIGN_AND_BREAK(lvl99_fp);
    }
    case 16:
    {
        ASSIGN_AND_BREAK(fp_maxes_at);
    }
    case 17:
    {
        ASSIGN_AND_BREAK(lvl99_tp);
    }
    case 18:
    {
        ASSIGN_AND_BREAK(tp_maxes_at);
    }
    case 19:
    {
        ASSIGN_AND_BREAK(lvl99_kick);
    }
    case 20:
    {
        ASSIGN_AND_BREAK(kick_maxes_at);
    }
    case 21:
    {
        ASSIGN_AND_BREAK(lvl99_body);
    }
    case 22:
    {
        ASSIGN_AND_BREAK(body_maxes_at);
    }
    case 23:
    {
        ASSIGN_AND_BREAK(lvl99_control);
    }
    case 24:
    {
        ASSIGN_AND_BREAK(control_maxes_at);
    }
    case 25:
    {
        ASSIGN_AND_BREAK(lvl99_guard);
    }
    case 26:
    {
        ASSIGN_AND_BREAK(guard_maxes_at);
    }
    case 27:
    {
        ASSIGN_AND_BREAK(lvl99_speed);
    }
    case 28:
    {
        ASSIGN_AND_BREAK(speed_maxes_at);
    }
    case 29:
    {
        ASSIGN_AND_BREAK(lvl99_stamina);
    }
    case 30:
    {
        ASSIGN_AND_BREAK(stamina_maxes_at);
    }
    case 31:
    {
        ASSIGN_AND_BREAK(lvl99_guts);
    }
    case 32:
    {
        ASSIGN_AND_BREAK(guts_maxes_at);
    }
    case 33:
    {
        ASSIGN_AND_BREAK(freedom);
    }
    case 34:
    {
        ASSIGN_AND_BREAK(total);
    }
    case 35:
    {
        ASSIGN_AND_BREAK(max_total);
    }
    case 36:
    {
        strncpy(player->move1_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise_string(player->move1_name, player->move1_name_normalised,
                             INA_FULLNAME_MAX_LEN);
        break;
    }
    case 37:
    {
        ASSIGN_AND_BREAK(move1_learns_at);
    }
    case 38:
    {
        strncpy(player->move2_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise_string(player->move2_name, player->move2_name_normalised,
                             INA_FULLNAME_MAX_LEN);
        break;
    }
    case 39:
    {
        ASSIGN_AND_BREAK(move2_learns_at);
    }
    case 40:
    {
        strncpy(player->move3_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise_string(player->move3_name, player->move3_name_normalised,
                             INA_FULLNAME_MAX_LEN);
        break;
    }
    case 41:
    {
        ASSIGN_AND_BREAK(move3_learns_at);
    }
    case 42:
    {
        strncpy(player->move4_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise_string(player->move4_name, player->move4_name_normalised,
                             INA_FULLNAME_MAX_LEN);
        break;
    }
    case 43:
    {
        ASSIGN_AND_BREAK(move4_learns_at);
        break;
    }
    case 44:
    {
        int id = strtol(cell_content, NULL, 16);

        player->hex_id = id;
        break;
    }
    default:
    {
        ina_errno = INA_ERRT_UNKNOWN;
        return 0;
    }
    }

    return 1;
}