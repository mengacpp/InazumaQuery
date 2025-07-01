#include "inazuma/inazuma.h"

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inazuma/core.h"
#include "inazuma/hmap.h"
#include "inazuma/player.h"
#include "inazuma/utils.h"

#include "inazuma/errno.h"

#define INA_PLAYER_DB_MAX_PLAYERS 2500

struct InaPlayerDB
{
    InaPlayer *players;
    size_t player_count;
    InaHmap *fullname_hmap;
    // uint16_t *nickname_index;
};

#define entry(x) INA_PLAYERDB_COL_TYPE_##x
typedef enum InaPlayerDBColType
{
    entry(FULLNAME) = 0,
    entry(NICKNAME),
    entry(POSITION),
    entry(GENDER),
    entry(SIZE),
    entry(ELEMENT),

    entry(LVL1_FP),
    entry(LVL1_TP),
    entry(LVL1_KICK),
    entry(LVL1_BODY),
    entry(LVL1_CONTROL), //
    entry(LVL1_GUARD),
    entry(LVL1_SPEED),
    entry(LVL1_STAMINA),
    entry(LVL1_GUTS),


    entry(LVL99_FP),
    entry(FP_MAXES_AT),

    entry(LVL99_TP),
    entry(TP_MAXES_AT),

    entry(LVL99_KICK),
    entry(KICK_MAXES_AT), //

    entry(LVL99_BODY),
    entry(BODY_MAXES_AT),

    entry(LVL99_CONTROL),
    entry(CONTROL_MAXES_AT),

    entry(LVL99_GUARD),
    entry(GUARD_MAXES_AT),

    entry(LVL99_SPEED),
    entry(SPEED_MAXES_AT),

    entry(LVL99_STAMINA),
    entry(STAMINA_MAXES_AT), //

    entry(LVL99_GUTS),
    entry(GUTS_MAXES_AT),

    entry(FREEDOM),
    entry(TOTAL),
    entry(MAX_TOTAL),


    entry(MOVE1),
    entry(MOVE1_LEARNS_AT),

    entry(MOVE2),
    entry(MOVE2_LEARNS_AT),

    entry(MOVE3), //
    entry(MOVE3_LEARNS_AT),

    entry(MOVE4),
    entry(MOVE4_LEARNS_AT),

    entry(PLAYER_HEX_ID),
    entry(INVALID)
} InaPlayerDBColType;
#undef entry

int parse_player_csv(InaPlayerDB *db, const char *csv_path);
int assign_to_player_by_col(InaPlayer *player, InaPlayerDBColType col,
                            char const *cell_content);
int get_stat_from_cell(char const *cell_content);

InaPlayerDB *ina_open_player_db(const char *csv_path)
{
    InaPlayerDB *db = malloc(sizeof(*db));
    if (!db)
    {
        ina_stderrno = errno;
        ina_errno = INA_ERRT_STDERROR;
        return NULL;
    }

    db->players = malloc(INA_PLAYER_DB_MAX_PLAYERS * sizeof(*db->players));
    if (!db->players)
    {
        ina_stderrno = errno;
        ina_errno = INA_ERRT_STDERROR;
        ina_close_player_db(&db);
        return NULL;
    }
    (*db).player_count = 0;

    db->fullname_hmap = ina_hmap_create(INA_PLAYER_DB_MAX_PLAYERS);

    if (!db->fullname_hmap)
    {
        ina_close_player_db(&db);
        return NULL;
    }

    if (!parse_player_csv(db, csv_path))
    {
        ina_close_player_db(&db);
        return NULL;
    }

    return db;
}

void ina_close_player_db(InaPlayerDB **db)
{
    if (!(*db)) return;

    free((*db)->players);
    ina_hmap_destroy(&(*db)->fullname_hmap);

    free(*db);

    *db = NULL;
}

InaPlayer *ina_get_players(InaPlayerDB const *db, size_t max_count,
                           size_t *count)
{
    if (db == NULL)
    {
        *count = 0;
        ina_errno = INA_ERRT_PARAM_NULL;
        return NULL;
    }

    *count = max_count > db->player_count ? db->player_count : max_count;

    InaPlayer *result = malloc(*count * sizeof(*result));

    for (size_t i = 0; i < (*count); ++i)
    {
        memcpy(&result[i], &db->players[i], sizeof(InaPlayer));
    }

    return result;
}

InaPlayer *ina_get_player_by_fullname(InaPlayerDB const *db,
                                      char const *fullname)
{
    char fullname_normalised[INA_FULLNAME_MAX_LEN];

    ina_normalise(fullname, fullname_normalised, INA_FULLNAME_MAX_LEN);

    bool found;
    unsigned int id =
        ina_hmap_get(db->fullname_hmap, fullname_normalised, &found);

    if (!found)
    {
        return NULL;
    }

    return &db->players[id];
}

int parse_player_csv(InaPlayerDB *db, const char *csv_path)
{
    char *csv_content = ina_read_file_content(csv_path);

    if (!csv_content) return INA_FAILURE;

    char *current = csv_content;

    size_t content_len = strlen(csv_content);


    size_t current_row = 0;
    size_t current_column = 0;

    for (size_t i = 0; i < content_len; ++i)
    {
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

        if (current_column >= (size_t)INA_PLAYERDB_COL_TYPE_INVALID)
        {
            ina_errno = INA_ERRT_CSV_STRUCTURE_INVALID;
            return INA_FAILURE;
        }


        if (!assign_to_player_by_col(&db->players[current_row],
                                     (InaPlayerDBColType)current_column,
                                     cell_content))
        {
            return INA_FAILURE;
        }

        if ((InaPlayerDBColType)current_column ==
            INA_PLAYERDB_COL_TYPE_FULLNAME)
        {
            char const *fullname_norm =
                db->players[current_row].fullname_normalised;

            if (!ina_hmap_add(db->fullname_hmap, fullname_norm, current_row))
            {
                return INA_FAILURE;
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

    db->player_count = current_row + 1;

    free(csv_content);

    return INA_SUCCESS;
}

#define assign_stat_and_break(stat)                                            \
    do                                                                         \
    {                                                                          \
        player->stat = get_stat_from_cell(cell_content);                       \
                                                                               \
        break;                                                                 \
    } while (0)

int assign_to_player_by_col(InaPlayer *player, InaPlayerDBColType col,
                            char const *cell_content)
{
    switch (col)
    {
    case INA_PLAYERDB_COL_TYPE_FULLNAME:
    {
        strncpy(player->fullname, cell_content, INA_FULLNAME_MAX_LEN);

        ina_normalise(player->fullname, player->fullname_normalised,
                      INA_FULLNAME_MAX_LEN);

        break;
    }
    case INA_PLAYERDB_COL_TYPE_NICKNAME:
    {
        strncpy(player->nickname, cell_content, INA_FULLNAME_MAX_LEN);

        ina_normalise(player->nickname, player->nickname_normalised,
                      INA_FULLNAME_MAX_LEN);
        break;
    }
    case INA_PLAYERDB_COL_TYPE_POSITION:
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
            return INA_FAILURE;
        }

        break;
    }
    case INA_PLAYERDB_COL_TYPE_GENDER:
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
            return INA_FAILURE;
        }

        break;
    }
    case INA_PLAYERDB_COL_TYPE_SIZE:
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
            return INA_FAILURE;
        }

        break;
    }
    case INA_PLAYERDB_COL_TYPE_ELEMENT:
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
            return INA_FAILURE;
        }

        break;
    }
    case INA_PLAYERDB_COL_TYPE_LVL1_FP:
    {
        assign_stat_and_break(lvl1_fp);
    }
    case INA_PLAYERDB_COL_TYPE_LVL1_TP:
    {
        assign_stat_and_break(lvl1_tp);
    }
    case INA_PLAYERDB_COL_TYPE_LVL1_KICK:
    {
        assign_stat_and_break(lvl1_kick);
    }
    case INA_PLAYERDB_COL_TYPE_LVL1_BODY:
    {
        assign_stat_and_break(lvl1_body);
    }
    case INA_PLAYERDB_COL_TYPE_LVL1_CONTROL:
    {
        assign_stat_and_break(lvl1_control);
    }
    case INA_PLAYERDB_COL_TYPE_LVL1_GUARD:
    {
        assign_stat_and_break(lvl1_guard);
    }
    case INA_PLAYERDB_COL_TYPE_LVL1_SPEED:
    {
        assign_stat_and_break(lvl1_speed);
    }
    case INA_PLAYERDB_COL_TYPE_LVL1_STAMINA:
    {
        assign_stat_and_break(lvl1_stamina);
    }
    case INA_PLAYERDB_COL_TYPE_LVL1_GUTS:
    {
        assign_stat_and_break(lvl1_guts);
    }
    case INA_PLAYERDB_COL_TYPE_LVL99_FP:
    {
        assign_stat_and_break(lvl99_fp);
    }
    case INA_PLAYERDB_COL_TYPE_FP_MAXES_AT:
    {
        assign_stat_and_break(fp_maxes_at);
    }
    case INA_PLAYERDB_COL_TYPE_LVL99_TP:
    {
        assign_stat_and_break(lvl99_tp);
    }
    case INA_PLAYERDB_COL_TYPE_TP_MAXES_AT:
    {
        assign_stat_and_break(tp_maxes_at);
    }
    case INA_PLAYERDB_COL_TYPE_LVL99_KICK:
    {
        assign_stat_and_break(lvl99_kick);
    }
    case INA_PLAYERDB_COL_TYPE_KICK_MAXES_AT:
    {
        assign_stat_and_break(kick_maxes_at);
    }
    case INA_PLAYERDB_COL_TYPE_LVL99_BODY:
    {
        assign_stat_and_break(lvl99_body);
    }
    case INA_PLAYERDB_COL_TYPE_BODY_MAXES_AT:
    {
        assign_stat_and_break(body_maxes_at);
    }
    case INA_PLAYERDB_COL_TYPE_LVL99_CONTROL:
    {
        assign_stat_and_break(lvl99_control);
    }
    case INA_PLAYERDB_COL_TYPE_CONTROL_MAXES_AT:
    {
        assign_stat_and_break(control_maxes_at);
    }
    case INA_PLAYERDB_COL_TYPE_LVL99_GUARD:
    {
        assign_stat_and_break(lvl99_guard);
    }
    case INA_PLAYERDB_COL_TYPE_GUARD_MAXES_AT:
    {
        assign_stat_and_break(guard_maxes_at);
    }
    case INA_PLAYERDB_COL_TYPE_LVL99_SPEED:
    {
        assign_stat_and_break(lvl99_speed);
    }
    case INA_PLAYERDB_COL_TYPE_SPEED_MAXES_AT:
    {
        assign_stat_and_break(speed_maxes_at);
    }
    case INA_PLAYERDB_COL_TYPE_LVL99_STAMINA:
    {
        assign_stat_and_break(lvl99_stamina);
    }
    case INA_PLAYERDB_COL_TYPE_STAMINA_MAXES_AT:
    {
        assign_stat_and_break(stamina_maxes_at);
    }
    case INA_PLAYERDB_COL_TYPE_LVL99_GUTS:
    {
        assign_stat_and_break(lvl99_guts);
    }
    case INA_PLAYERDB_COL_TYPE_GUTS_MAXES_AT:
    {
        assign_stat_and_break(guts_maxes_at);
    }
    case INA_PLAYERDB_COL_TYPE_FREEDOM:
    {
        assign_stat_and_break(freedom);
    }
    case INA_PLAYERDB_COL_TYPE_TOTAL:
    {
        assign_stat_and_break(total);
    }
    case INA_PLAYERDB_COL_TYPE_MAX_TOTAL:
    {
        assign_stat_and_break(max_total);
    }
    case INA_PLAYERDB_COL_TYPE_MOVE1:
    {
        strncpy(player->move1_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise(player->move1_name, player->move1_name_normalised,
                      INA_FULLNAME_MAX_LEN);
        break;
    }
    case INA_PLAYERDB_COL_TYPE_MOVE1_LEARNS_AT:
    {
        assign_stat_and_break(move1_learns_at);
    }
    case INA_PLAYERDB_COL_TYPE_MOVE2:
    {
        strncpy(player->move2_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise(player->move2_name, player->move2_name_normalised,
                      INA_FULLNAME_MAX_LEN);
        break;
    }
    case INA_PLAYERDB_COL_TYPE_MOVE2_LEARNS_AT:
    {
        assign_stat_and_break(move2_learns_at);
    }
    case INA_PLAYERDB_COL_TYPE_MOVE3:
    {
        strncpy(player->move3_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise(player->move3_name, player->move3_name_normalised,
                      INA_FULLNAME_MAX_LEN);
        break;
    }
    case INA_PLAYERDB_COL_TYPE_MOVE3_LEARNS_AT:
    {
        assign_stat_and_break(move3_learns_at);
    }
    case INA_PLAYERDB_COL_TYPE_MOVE4:
    {
        strncpy(player->move4_name, cell_content, INA_FULLNAME_MAX_LEN);
        ina_normalise(player->move4_name, player->move4_name_normalised,
                      INA_FULLNAME_MAX_LEN);
        break;
    }
    case INA_PLAYERDB_COL_TYPE_MOVE4_LEARNS_AT:
    {
        assign_stat_and_break(move4_learns_at);
        break;
    }
    case INA_PLAYERDB_COL_TYPE_PLAYER_HEX_ID:
    {
        int id = ina_string_to_int(cell_content, 16);

        player->hex_id = id;
        break;
    }
    case INA_PLAYERDB_COL_TYPE_INVALID:
    {
        ina_errno = INA_ERRT_CSV_STRUCTURE_INVALID;
        return INA_FAILURE;
    }
    default:
    {
        INA_NOT_IMPLEMENTED;
        ina_errno = INA_ERRT_UNKNOWN;
        return INA_FAILURE;
    }
    }

    return INA_SUCCESS;
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