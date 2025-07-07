#include "InazumaQuery/move.h"

void ina_move_fprint(FILE *out, ina_move_t const *mv)
{
    fprintf(out, "%s POW %i", mv->name, mv->base_power);
}