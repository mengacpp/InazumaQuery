#include <stdio.h>
#include <time.h>

#include "InazumaQuery/core/errno.h"
#include "InazumaQuery/inazuma.h" // IWYU pragma: keep
#include "InazumaQuery/player.h"

int main()
{
    char const *csv_path = INA_DEBUG_DATA_DIR "/players/ie2.csv";
    ina_pdb_t *db = ina_pdb_create(csv_path);

    if (!db)
    {
        ina_perror("Database not open");
        ina_pdb_destroy(&db);
        return 1;
    }

    ina_player_t const *axel_blaze = ina_pdb_find_fullname(db, "Axel Blaze");

    if (!axel_blaze)
    {
        ina_perror("Move not found");
        ina_pdb_destroy(&db);
        return 1;
    }

    ina_player_fprint(stdout, axel_blaze);


    ina_pdb_destroy(&db);
    return 0;
}