#include <stdio.h>

#include "InazumaQuery/core/errno.h"
#include "InazumaQuery/db.h"
#include "InazumaQuery/inazuma.h" // IWYU pragma: keep

int main()
{
    ina_db_t *db =
        ina_db_create_from_csv(INA_DEBUG_DATA_DIR "/players/ie3.csv");

    if (!db)
    {
        fprintf(stderr, "Failed to open database: %s", ina_strerrno(ina_errno));
        ina_db_close(&db);
        return 1;
    }

    ina_db_close(&db);
    return 0;
}