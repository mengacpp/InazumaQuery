#ifndef INAZUMA_PLAYER_H_
#define INAZUMA_PLAYER_H_

#include "InazumaQuery/core/core.h"
#include <stdio.h>

#define INA_FULLNAME_MAX_LEN                                                   \
    25 // seems to be 22 in ie3. no data for other databases

#include <stdint.h>

typedef enum
{
    PLAYER_POSITION_GK = 0,
    PLAYER_POSITION_DF,
    PLAYER_POSITION_MF,
    PLAYER_POSITION_FW
} ina_player_pos_t;

typedef enum
{
    PLAYER_GENDER_MALE = 0,
    PLAYER_GENDER_FEMALE
} ina_player_gender_t;

typedef enum
{
    PLAYER_SIZE_SMALL,
    PLAYER_SIZE_MEDIUM,
    PLAYER_SIZE_LARGE
} ina_player_size_t;

typedef enum
{
    ELEMENT_EARTH,
    ELEMENT_AIR,
    ELEMENT_FIRE,
    ELEMENT_WOOD
} ina_player_element_t;


typedef struct
{
    char fullname[INA_FULLNAME_MAX_LEN];
    char fullname_normalised[INA_FULLNAME_MAX_LEN];
    char nickname[INA_FULLNAME_MAX_LEN];
    char nickname_normalised[INA_FULLNAME_MAX_LEN];

    ina_player_pos_t position;
    ina_player_gender_t gender;
    ina_player_size_t size;
    ina_player_element_t element;

    uint8_t lvl99_fp, lvl99_tp, lvl99_kick, lvl99_body, lvl99_control,
        lvl99_guard, lvl99_speed, lvl99_stamina, lvl99_guts;
    uint8_t freedom;

    uint8_t lvl1_fp, lvl1_tp, lvl1_kick, lvl1_body, lvl1_control, lvl1_guard,
        lvl1_speed, lvl1_stamina, lvl1_guts;

    uint8_t fp_maxes_at, tp_maxes_at, kick_maxes_at, body_maxes_at,
        control_maxes_at, guard_maxes_at, speed_maxes_at, stamina_maxes_at,
        guts_maxes_at;

    uint8_t total, max_total;

    char move1_name[INA_FULLNAME_MAX_LEN];
    char move1_name_normalised[INA_FULLNAME_MAX_LEN];
    uint8_t move1_learns_at;

    char move2_name[INA_FULLNAME_MAX_LEN];
    char move2_name_normalised[INA_FULLNAME_MAX_LEN];
    uint8_t move2_learns_at;

    char move3_name[INA_FULLNAME_MAX_LEN];
    char move3_name_normalised[INA_FULLNAME_MAX_LEN];
    uint8_t move3_learns_at;

    char move4_name[INA_FULLNAME_MAX_LEN];
    char move4_name_normalised[INA_FULLNAME_MAX_LEN];
    uint8_t move4_learns_at;

    uint16_t hex_id;
} ina_player_t;

// TODO delete this enum. it is not needed
#define entry(x) INA_PLAYER_ATTRIBUTE_##x
typedef enum ina_player_attribute_t
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
    entry(COUNT)
} ina_player_attribute_t;
#undef entry

INA_BEGIN_EXTERN_C

INA_API void ina_player_list_printer(FILE *out, void const *p);
INA_API void ina_player_fprint(FILE *out, ina_player_t const *p);

INA_END_EXTERN_C


#endif // INAZUMA_PLAYER_H_