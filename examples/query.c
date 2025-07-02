#include "inazuma/inazuma.h" // IWYU pragma: keep


// error handlings is omitted in this example to make it easier to read

// function to compare two players: returns > 0 if p > p2, == 0 if p == p2 and <
// 0 if p < p2
// in this case we are comparing them in a way that p > p2 if p->lvl99_kick >
// p2->lvl99_kick
int compare_kick(InaPlayer const *p, InaPlayer const *p2)
{
    return p2->lvl99_kick - p->lvl99_kick;
}

// filter function to check if a player has a specific requirement
// in this case whe are checking that the player is of type air.
bool filter_air_element(InaPlayer const *p)
{
    return p->element == ELEMENT_AIR;
}

int main()
{
    // open the Inazuma eleven 3 player database
    InaPlayerDB *ie3_db = ina_player_db_open(INA_DATA_PLAYER_IE3_DIR);

    // from the ie3 database, find players that respect the filter_air_element
    // condition and sort them using the compare_kick
    // in simple terms, we are getting all players of element air and sorting
    // them by their kick stat at lvl99.
    InaList *players =
        ina_player_db_get(ie3_db, 0, filter_air_element, compare_kick);


    // print all players found
    for (size_t i = 0; i < ina_list_count(players); ++i)
    {
        ina_player_print((InaPlayer *)ina_list_at(players, i));
    }

    // cleanup
    ina_list_destroy(&players);
    ina_player_db_close(&ie3_db);

    return 0;
}