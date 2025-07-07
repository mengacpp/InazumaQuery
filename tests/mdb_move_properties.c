#include <stdio.h>
#include <time.h>

#include "InazumaQuery/core/errno.h"
#include "InazumaQuery/inazuma.h" // IWYU pragma: keep

int main()
{
    char const *csv_path = INA_DEBUG_DATA_DIR "/moves/ie2.csv";
    ina_mdb_t *db = ina_mdb_create(csv_path);

    if (!db)
    {
        ina_perror("Database not open");
        ina_mdb_destroy(&db);
        return 1;
    }

    ina_move_t const *killer_slide = ina_mdb_find_name(db, "Killer slide");

    if (!killer_slide)
    {
        ina_perror("Move not found");
        ina_mdb_destroy(&db);
        return 1;
    }

    ina_move_fprint(stdout, killer_slide);


    ina_mdb_destroy(&db);
    return 0;
}