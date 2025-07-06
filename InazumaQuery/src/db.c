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


// TODO this file really needs refactor
// this is a mess, should break into smaller functions maybe / ensure better
// error checking
// csv files should be parsed into an intermediate state easier to parse
// (subdivide it in cols and cels)


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

int assign_to_player_by_col(ina_player_t *player, ina_player_attribute_t col,
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

    // TODO fix this to make it dynamically allocated
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

// TODO maybe create a list of pointers instead of players? not sure if its good
// tho may risk that the user modify the database
INA_API ina_list_t *ina_db_query(ina_db_t const *db, uint16_t max_count,
                                 ina_player_filter_fn_t filter_fn,
                                 ina_player_compare_fn compare_fn)
{
    if (!compare_fn)
        g_compare_fn = ina_db_default_compare;
    else
        g_compare_fn = compare_fn;

    if (!filter_fn)
        g_filter_fn = ina_db_accept_all;
    else
        g_filter_fn = filter_fn;

    if (!db)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return NULL;
    }

    ina_list_t *result = ina_list_create(sizeof(ina_player_t));

    if (max_count == 0) max_count = ina_list_count(db->players);

    ina_list_t *filtered = ina_filter(db->players, filter_wrapper);

    ina_sort(filtered, compare_wrapper, set);

    size_t count = max_count > ina_list_count(filtered)
                       ? ina_list_count(filtered)
                       : max_count;

    // TODO implement algorithm to copy this in an efficent way
    for (size_t i = 0; i < count; ++i)
    {
        ina_player_t *p = ina_list_at(filtered, i);
        ina_list_add(result, p);
    }

    ina_list_destroy(&filtered);

    return result;
}

ina_player_t const *ina_db_query_fullname(ina_db_t const *db,
                                          char const *fullname)
{
    char fullname_normalised[INA_FULLNAME_MAX_LEN];

    ina_normalise(fullname, fullname_normalised, INA_FULLNAME_MAX_LEN);

    bool found;
    unsigned int id =
        ina_hash_map_get(db->fullname_hmap, fullname_normalised, &found);

    if (!found) return NULL;

    return ina_list_at(db->players, id);
}

#define assign_stat_and_break(stat)                                            \
    do                                                                         \
    {                                                                          \
        player->stat = get_stat_from_cell(cell_content);                       \
                                                                               \
        break;                                                                 \
    } while (0)

int assign_to_player_by_col(ina_player_t *player, ina_player_attribute_t col,
                            char const *cell_content)
{
    switch (col)
    {
    case INA_PLAYER_ATTRIBUTE_FULLNAME:
    {
        strncpy(player->fullname, cell_content, INA_FULLNAME_MAX_LEN);

        ina_normalise(player->fullname, player->fullname_normalised,
                      INA_FULLNAME_MAX_LEN);

        break;
    }
    case INA_PLAYER_ATTRIBUTE_NICKNAME:
    {
        strncpy(player->nickname, cell_content, INA_FULLNAME_MAX_LEN);

        ina_normalise(player->nickname, player->nickname_normalised,
                      INA_FULLNAME_MAX_LEN);
        break;
    }
    case INA_PLAYER_ATTRIBUTE_POSITION:
    {
        int is_gk = ina_normalise_strcmp("GK", cell_content) == 0;
        int is_df = ina_normalise_strcmp("DF", cell_content) == 0;
        int is_mf = ina_normalise_strcmp("MF", cell_content) == 0;
        int is_fw = ina_normalise_strcmp("FW", cell_content) == 0;

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
    case INA_PLAYER_ATTRIBUTE_GENDER:
    {
        int is_male = ina_normalise_strcmp("male", cell_content) == 0;
        int is_female = ina_normalise_strcmp("female", cell_content) == 0;

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
    case INA_PLAYER_ATTRIBUTE_SIZE:
    {
        int is_small = ina_normalise_strcmp("small", cell_content) == 0;
        int is_medium = ina_normalise_strcmp("medium", cell_content) == 0;
        int is_large = ina_normalise_strcmp("large", cell_content) == 0;

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
    case INA_PLAYER_ATTRIBUTE_ELEMENT:
    {
        int is_air = ina_normalise_strcmp("air", cell_content) == 0;
        int is_earth = ina_normalise_strcmp("earth", cell_content) == 0;
        int is_fire = ina_normalise_strcmp("fire", cell_content) == 0;
        int is_wood = ina_normalise_strcmp("wood", cell_content) == 0;

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
    case INA_PLAYER_ATTRIBUTE_LVL1_FP:
    {
        assign_stat_and_break(lvl1_fp);
    }
    case INA_PLAYER_ATTRIBUTE_LVL1_TP:
    {
        assign_stat_and_break(lvl1_tp);
    }
    case INA_PLAYER_ATTRIBUTE_LVL1_KICK:
    {
        assign_stat_and_break(lvl1_kick);
    }
    case INA_PLAYER_ATTRIBUTE_LVL1_BODY:
    {
        assign_stat_and_break(lvl1_body);
    }
    case INA_PLAYER_ATTRIBUTE_LVL1_CONTROL:
    {
        assign_stat_and_break(lvl1_control);
    }
    case INA_PLAYER_ATTRIBUTE_LVL1_GUARD:
    {
        assign_stat_and_break(lvl1_guard);
    }
    case INA_PLAYER_ATTRIBUTE_LVL1_SPEED:
    {
        assign_stat_and_break(lvl1_speed);
    }
    case INA_PLAYER_ATTRIBUTE_LVL1_STAMINA:
    {
        assign_stat_and_break(lvl1_stamina);
    }
    case INA_PLAYER_ATTRIBUTE_LVL1_GUTS:
    {
        assign_stat_and_break(lvl1_guts);
    }
    case INA_PLAYER_ATTRIBUTE_LVL99_FP:
    {
        assign_stat_and_break(lvl99_fp);
    }
    case INA_PLAYER_ATTRIBUTE_FP_MAXES_AT:
    {
        assign_stat_and_break(fp_maxes_at);
    }
    case INA_PLAYER_ATTRIBUTE_LVL99_TP:
    {
        assign_stat_and_break(lvl99_tp);
    }
    case INA_PLAYER_ATTRIBUTE_TP_MAXES_AT:
    {
        assign_stat_and_break(tp_maxes_at);
    }
    case INA_PLAYER_ATTRIBUTE_LVL99_KICK:
    {
        assign_stat_and_break(lvl99_kick);
    }
    case INA_PLAYER_ATTRIBUTE_KICK_MAXES_AT:
    {
        assign_stat_and_break(kick_maxes_at);
    }
    case INA_PLAYER_ATTRIBUTE_LVL99_BODY:
    {
        assign_stat_and_break(lvl99_body);
    }
    case INA_PLAYER_ATTRIBUTE_BODY_MAXES_AT:
    {
        assign_stat_and_break(body_maxes_at);
    }
    case INA_PLAYER_ATTRIBUTE_LVL99_CONTROL:
    {
        assign_stat_and_break(lvl99_control);
    }
    case INA_PLAYER_ATTRIBUTE_CONTROL_MAXES_AT:
    {
        assign_stat_and_break(control_maxes_at);
    }
    case INA_PLAYER_ATTRIBUTE_LVL99_GUARD:
    {
        assign_stat_and_break(lvl99_guard);
    }
    case INA_PLAYER_ATTRIBUTE_GUARD_MAXES_AT:
    {
        assign_stat_and_break(guard_maxes_at);
    }
    case INA_PLAYER_ATTRIBUTE_LVL99_SPEED:
    {
        assign_stat_and_break(lvl99_speed);
    }
    case INA_PLAYER_ATTRIBUTE_SPEED_MAXES_AT:
    {
        assign_stat_and_break(speed_maxes_at);
    }
    case INA_PLAYER_ATTRIBUTE_LVL99_STAMINA:
    {
        assign_stat_and_break(lvl99_stamina);
    }
    case INA_PLAYER_ATTRIBUTE_STAMINA_MAXES_AT:
    {
        assign_stat_and_break(stamina_maxes_at);
    }
    case INA_PLAYER_ATTRIBUTE_LVL99_GUTS:
    {
        assign_stat_and_break(lvl99_guts);
    }
    case INA_PLAYER_ATTRIBUTE_GUTS_MAXES_AT:
    {
        assign_stat_and_break(guts_maxes_at);
    }
    case INA_PLAYER_ATTRIBUTE_FREEDOM:
    {
        assign_stat_and_break(freedom);
    }
    case INA_PLAYER_ATTRIBUTE_TOTAL:
    {
        assign_stat_and_break(total);
    }
    case INA_PLAYER_ATTRIBUTE_MAX_TOTAL:
    {
        assign_stat_and_break(max_total);
    }
    case INA_PLAYER_ATTRIBUTE_MOVE1:
    {
        strncpy(player->move1_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise(player->move1_name, player->move1_name_normalised,
                      INA_FULLNAME_MAX_LEN);
        break;
    }
    case INA_PLAYER_ATTRIBUTE_MOVE1_LEARNS_AT:
    {
        assign_stat_and_break(move1_learns_at);
    }
    case INA_PLAYER_ATTRIBUTE_MOVE2:
    {
        strncpy(player->move2_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise(player->move2_name, player->move2_name_normalised,
                      INA_FULLNAME_MAX_LEN);
        break;
    }
    case INA_PLAYER_ATTRIBUTE_MOVE2_LEARNS_AT:
    {
        assign_stat_and_break(move2_learns_at);
    }
    case INA_PLAYER_ATTRIBUTE_MOVE3:
    {
        strncpy(player->move3_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise(player->move3_name, player->move3_name_normalised,
                      INA_FULLNAME_MAX_LEN);
        break;
    }
    case INA_PLAYER_ATTRIBUTE_MOVE3_LEARNS_AT:
    {
        assign_stat_and_break(move3_learns_at);
    }
    case INA_PLAYER_ATTRIBUTE_MOVE4:
    {
        strncpy(player->move4_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise(player->move4_name, player->move4_name_normalised,
                      INA_FULLNAME_MAX_LEN);
        break;
    }
    case INA_PLAYER_ATTRIBUTE_MOVE4_LEARNS_AT:
    {
        assign_stat_and_break(move4_learns_at);
        break;
    }
    case INA_PLAYER_ATTRIBUTE_PLAYER_HEX_ID:
    {
        int id = ina_string_to_int(cell_content, 16);

        player->hex_id = id;
        break;
    }
    case INA_PLAYER_ATTRIBUTE_COUNT:
    {
        ina_errno = INA_ERRT_CSV_STRUCTURE_INVALID;
        return 0;
    }
    default:
    {
        INA_NOT_IMPLEMENTED;
        ina_errno = INA_ERRT_UNKNOWN;
        return 0;
    }
    }

    return 1;
}

int get_stat_from_cell(char const *cell_content)
{
    if (ina_normalise_strcmp("event", cell_content) == 0)
    {
        return -1;
    }

    int stat = 0;

    if (strlen(cell_content) < 1)
    {
        return 0;
    }

    stat = ina_string_to_int(cell_content, 10);

    if (stat < 0)
    {
        ina_errno = INA_ERRT_CSV_CELL_INVALID;
        return -2;
    }

    return stat;
}