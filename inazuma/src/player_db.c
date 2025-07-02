#include "inazuma/player_db.h"

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inazuma/core/errno.h"

#include "inazuma/player.h"

#include "inazuma/utils/filter.h"
#include "inazuma/utils/hmap.h"
#include "inazuma/utils/list.h"
#include "inazuma/utils/sort.h"
#include "inazuma/utils/utils.h"


// TODO this file really needs refactor
// this is a mess, should break into smaller functions maybe / ensure better
// error checking
// csv files should be parsed into an intermediate state easier to parse
// (subdivide it in cols and cels)


#define INA_PLAYER_DB_MAX_PLAYERS 2500

INA_API inline bool ina_no_filtering(InaPlayer const *e)
{
    return true;
};

INA_API inline int ina_no_sorting(InaPlayer const *e)
{
    return e->hex_id;
};


struct InaPlayerDB
{
    InaList *players;
    InaHmap *fullname_hmap;
};

int parse_player_csv(InaPlayerDB *db, const char *csv_path);
int assign_to_player_by_col(InaPlayer *player, InaPlayerAttribute col,
                            char const *cell_content);
int get_stat_from_cell(char const *cell_content);

InaPlayerDB *ina_player_db_open(const char *csv_path)
{
    InaPlayerDB *db = malloc(sizeof(*db));
    if (!db)
    {
        ina_stderrno = errno;
        ina_errno = INA_ERRT_STDERROR;
        return NULL;
    }

    db->players = ina_list_create(sizeof(InaPlayer));
    if (!db->players)
    {
        ina_player_db_close(&db);
        return NULL;
    }

    db->fullname_hmap = ina_hmap_create(INA_PLAYER_DB_MAX_PLAYERS);

    if (!db->fullname_hmap)
    {
        ina_player_db_close(&db);
        return NULL;
    }

    if (!parse_player_csv(db, csv_path))
    {
        ina_player_db_close(&db);
        return NULL;
    }

    return db;
}

void ina_player_db_close(InaPlayerDB **db)
{
    if (!(*db)) return;

    ina_list_destroy(&(*db)->players);
    ina_hmap_destroy(&(*db)->fullname_hmap);

    free(*db);

    *db = NULL;
}


static InaPlayerFilterFn g_filter_fn;
static InaPlayerGetComparatorFn g_get_comparator_fn;

bool filter_wrapper(void const *e)
{
    return g_filter_fn((void const *)e);
}

int get_comparator_wrapper(void const *e)
{
    return g_get_comparator_fn((void const *)e);
}

void set(void *e, void const *e2)
{
    InaPlayer *player = e;

    InaPlayer const *player2 = e2;

    memcpy(player, player2, sizeof(InaPlayer));
}


INA_API InaList *ina_player_db_get(InaPlayerDB const *db, uint16_t max_count,
                                   InaPlayerFilterFn filter_fn,
                                   InaPlayerGetComparatorFn get_comparator_fn,
                                   InaSortDir sort_dir)
{
    if (!get_comparator_fn)
        g_get_comparator_fn = ina_no_sorting;
    else
        g_get_comparator_fn = get_comparator_fn;

    if (!filter_fn)
        g_filter_fn = ina_no_filtering;
    else
        g_filter_fn = filter_fn;

    InaList *result = ina_list_create(sizeof(InaPlayer));
    if (!db)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return NULL;
    }

    if (max_count == 0) max_count = ina_list_count(db->players);

    InaList *filtered = ina_filter(db->players, filter_wrapper);

    ina_sort(filtered, get_comparator_wrapper, set, sort_dir);

    size_t count = max_count > ina_list_count(filtered)
                       ? ina_list_count(filtered)
                       : max_count;

    // TODO implement algorithm to copy this in an efficent way
    for (size_t i = 0; i < count; ++i)
    {
        InaPlayer *p = ina_list_at(filtered, i);
        ina_list_add(result, p);
    }

    ina_list_destroy(&filtered);
    printf("filtering\n");

    return result;
}

InaPlayer const *ina_player_db_get_by_fullname(InaPlayerDB const *db,
                                               char const *fullname)
{
    char fullname_normalised[INA_FULLNAME_MAX_LEN];

    ina_normalise(fullname, fullname_normalised, INA_FULLNAME_MAX_LEN);

    bool found;
    unsigned int id =
        ina_hmap_get(db->fullname_hmap, fullname_normalised, &found);

    if (!found) return NULL;

    return ina_list_at(db->players, id);
}

int parse_player_csv(InaPlayerDB *db, const char *csv_path)
{
    char *csv_content = ina_read_file_content(csv_path);

    if (!csv_content) return 0;

    char *current = csv_content;

    size_t content_len = strlen(csv_content);


    size_t current_row = 0;
    size_t current_column = 0;

    for (size_t i = 0; i < content_len; ++i)
    {
        if (current_column == 0)
        {
            InaPlayer p;
            ina_list_add(db->players, &p);
        }
        size_t cell_len = 0;
        char *start_ptr = current;

        while (i < content_len && *current != ',' && *current != '\n')
        {
            ++current;
            ++cell_len;
            ++i;
        }

        char cell_content[cell_len + 1];
        memcpy(cell_content, start_ptr, cell_len);
        cell_content[cell_len] = '\0';

        if (current_column >= INA_PLAYER_ATTRIBUTE_COUNT)
        {
            ina_errno = INA_ERRT_CSV_STRUCTURE_INVALID;
            return 0;
        }


        if (!assign_to_player_by_col(
                (InaPlayer *)ina_list_at(db->players, current_row),
                (InaPlayerAttribute)current_column, cell_content))
        {
            return 0;
        }

        if ((InaPlayerAttribute)current_column == INA_PLAYER_ATTRIBUTE_FULLNAME)
        {
            char const *fullname_norm =
                ((InaPlayer *)ina_list_at(db->players, current_row))
                    ->fullname_normalised;

            if (!ina_hmap_add(db->fullname_hmap, fullname_norm, current_row))
            {
                return 0;
            }
        }

        // increment counters

        if (*current == ',')
        {
            current_column++;
        }
        if (*current == '\n')
        {
            current_column = 0;
            current_row++;
        }
        current++;
    }

    free(csv_content);

    return 1;
}

#define assign_stat_and_break(stat)                                            \
    do                                                                         \
    {                                                                          \
        player->stat = get_stat_from_cell(cell_content);                       \
                                                                               \
        break;                                                                 \
    } while (0)

int assign_to_player_by_col(InaPlayer *player, InaPlayerAttribute col,
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

    char only_digits[strlen(cell_content) + 1];

    ina_retrieve_digits(cell_content, only_digits, strlen(cell_content) + 1);

    if (strlen(only_digits) < 1)
    {
        ina_errno = INA_ERRT_CSV_CELL_INVALID;
        return -2;
    }

    stat = ina_string_to_int(only_digits, 10);

    if (stat < 0)
    {
        ina_errno = INA_ERRT_CSV_CELL_INVALID;
        return -2;
    }

    return stat;
}