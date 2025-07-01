#include "inazuma/player.h"
#include <stdio.h>

void ina_player_print(InaPlayer const *p)
{
    printf("%-25s ID=%#x\n", p->fullname, p->hex_id);
    printf("FP  %*i                 TP   %*i\n", 5, p->lvl99_fp, 5,
           p->lvl99_tp);
    printf("KIC %*i                 BOD  %*i\n", 5, p->lvl99_kick, 5,
           p->lvl99_body);
    printf("CON %*i                 GUA  %*i\n", 5, p->lvl99_control, 5,
           p->lvl99_guard);
    printf("SPE %*i                 STA  %*i\n", 5, p->lvl99_speed, 5,
           p->lvl99_stamina);
    printf("GUT %*i                 FRE  %*i\n", 5, p->lvl99_guts, 5,
           p->freedom);
}

/*
    fp, tp,
    kick, body,
    control, guard,
    speed, stamina,
    guts; freedom;

*/