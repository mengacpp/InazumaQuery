#include "InazumaQuery/inazuma.h" // IWYU pragma: keep


// error handlings is omitted in this example to make it easier to read

// function to compare two players: returns > 0 if p > p2, == 0 if p == p2 and <
// 0 if p < p2
// in this case we are comparing them in a way that p > p2 if p->lvl99_kick >
// p2->lvl99_kick
int compare_kick(ina_player_t const *p, ina_player_t const *p2)
{
    return p2->lvl99_kick - p->lvl99_kick;
}

// filter function to check if a player has a specific requirement
// in this case whe are checking that the player is of type air.
bool filter_air_element(ina_player_t const *p)
{
    return p->element == ELEMENT_AIR;
}

int main()
{
    // open the Inazuma eleven 3 player database
    ina_db_t *ie3_db = ina_db_create_from_csv(INA_IE3_CSV);

    // from the ie3 database, find players that respect the filter_air_element
    // condition and sort them using the compare_kick
    // in simple terms, we are getting all players of element air and sorting
    // them by their kick stat at lvl99.
    ina_list_t *players =
        ina_db_query(ie3_db, 0, filter_air_element, compare_kick);


    // print all players found
    ina_list_fprint(players, stdout, ina_player_list_printer, "\n");

    // cleanup
    ina_list_destroy(&players);
    ina_db_close(&ie3_db);

    return 0;
}