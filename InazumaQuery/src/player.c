#include "InazumaQuery/player.h"

INA_API void ina_player_fprint(FILE *out, ina_player_t const *p)
{
    fprintf(out, "%-25s ID=%#x\n", p->fullname, p->hex_id);
    fprintf(out, "FP  %*i                 TP   %*i\n", 5, p->lvl99_fp, 5,
            p->lvl99_tp);
    fprintf(out, "KIC %*i                 BOD  %*i\n", 5, p->lvl99_kick, 5,
            p->lvl99_body);
    fprintf(out, "CON %*i                 GUA  %*i\n", 5, p->lvl99_control, 5,
            p->lvl99_guard);
    fprintf(out, "SPE %*i                 STA  %*i\n", 5, p->lvl99_speed, 5,
            p->lvl99_stamina);
    fprintf(out, "GUT %*i                 FRE  %*i\n", 5, p->lvl99_guts, 5,
            p->freedom);
}