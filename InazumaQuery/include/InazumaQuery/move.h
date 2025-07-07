#ifndef INAZUMAQUERY_MOVE_H_
#define INAZUMAQUERY_MOVE_H_

#include "InazumaQuery/core/core.h"

#include <stdint.h>
#include <stdio.h>

#include "InazumaQuery/element.h"

typedef enum
{
    INA_MOVE_TYPE_BLOCK = 0,
    INA_MOVE_TYPE_DRIBBLE,
    INA_MOVE_TYPE_KEEPER,
    INA_MOVE_TYPE_SHOOT,
} ina_move_type_t;

typedef enum
{
    INA_MOVE_SECONDARY_TYPE_NONE = 0,
    INA_MOVE_SECONDARY_TYPE_SHOTBLOCK,
    INA_MOVE_SECONDARY_TYPE_LONGSHOT,
} ina_move_secondary_type_t;

typedef enum
{
    INA_MOVE_EVOLUTION_TYPE_L_SLOW = 0,
    INA_MOVE_EVOLUTION_TYPE_L_MEDIUM,
    INA_MOVE_EVOLUTION_TYPE_L_FAST,
    INA_MOVE_EVOLUTION_TYPE_SHIN_SLOW,
    INA_MOVE_EVOLUTION_TYPE_SHIN_MEDIUM,
    INA_MOVE_EVOLUTION_TYPE_SHIN_FAST,
} ina_move_evolution_type_t;

typedef struct
{
    char name[INA_NAME_MAX_LEN];
    char name_normalised[INA_NAME_MAX_LEN];

    ina_move_type_t type;
    ina_element_t element;

    float fouls_rate;
    uint8_t base_power;
    uint8_t max_power;
    uint8_t tp_cost;
    uint16_t hex_id;

    ina_move_evolution_type_t evolution_type;
    ina_move_secondary_type_t secondary_type;
} ina_move_t;

INA_BEGIN_EXTERN_C

INA_API void ina_move_fprint(FILE *out, ina_move_t const *mv);

INA_END_EXTERN_C

#endif // INAZUMAQUERY_MOVE_H_