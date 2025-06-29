#ifndef __INAZUMA_PLAYER_H__
#define __INAZUMA_PLAYER_H__

#define IZM_FULLNAME_MAX_LEN 25

#include <stdint.h>

typedef enum
{
    POSITION_GK = 0,
    POSITION_DF,
    POSITION_MF,
    POSITION_FW
} InazumaPlayerPosition;

typedef enum
{
    GENDER_MALE = 0,
    GENDER_FEMALE
} InazumaPlayerGender;

typedef enum
{
    SIZE_SMALL,
    SIZE_MEDIUM,
    SIZE_LARGE
} InazumaPlayerSize;

typedef enum
{
    ELEMENT_EARTH,
    ELEMENT_AIR,
    ELEMENT_FIRE,
    ELEMENT_WOOD
} InazumaPlayerElement;


typedef struct
{
    char fullname[IZM_FULLNAME_MAX_LEN];
    char fullname_normalised[IZM_FULLNAME_MAX_LEN];
    char nickname[IZM_FULLNAME_MAX_LEN];
    char nickname_normalised[IZM_FULLNAME_MAX_LEN];

    InazumaPlayerPosition position;
    InazumaPlayerGender gender;
    InazumaPlayerSize size;
    InazumaPlayerElement element;

    uint8_t lvl99_fp, lvl99_tp, lvl99_kick, lvl99_body, lvl99_control,
        lvl99_guard, lvl99_speed, lvl99_stamina, lvl99_guts;
    uint8_t freedom;

    uint8_t lvl1_fp, lvl1_tp, lvl1_kick, lvl1_body, lvl1_control, lvl1_guard,
        lvl1_speed, lvl1_stamina, lvl1_guts;

    uint8_t fp_maxes_at, tp_maxes_at, kick_maxes_at, body_maxes_at,
        control_maxes_at, guard_maxes_at, speed_maxes_at, stamina_maxes_at,
        guts_maxes_at;

    uint8_t total, max_total;

    char move1_name[IZM_FULLNAME_MAX_LEN];
    char move1_name_normalised[IZM_FULLNAME_MAX_LEN];
    uint8_t move1_learns_at;

    char move2_name[IZM_FULLNAME_MAX_LEN];
    char move2_name_normalised[IZM_FULLNAME_MAX_LEN];
    uint8_t move2_learns_at;

    char move3_name[IZM_FULLNAME_MAX_LEN];
    char move3_name_normalised[IZM_FULLNAME_MAX_LEN];
    uint8_t move3_learns_at;

    char move4_name[IZM_FULLNAME_MAX_LEN];
    char move4_name_normalised[IZM_FULLNAME_MAX_LEN];
    uint8_t move4_learns_at;

    uint16_t hex_id;
} InazumaPlayerInfos;

#endif // __INAZUMA_PLAYER_H__