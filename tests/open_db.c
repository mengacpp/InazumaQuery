#include <stdio.h>

#include "inazuma/core/errno.h"
#include "inazuma/db.h"
#include "inazuma/inazuma.h" // IWYU pragma: keep

int main()
{
    ina_db_t *db = ina_db_create_from_csv(INA_IE3_CSV);

    if (!db)
    {
        fprintf(stderr, "Failed to open database: %s", ina_strerrno(ina_errno));
        ina_db_close(&db);
        return 1;
    }

    ina_db_close(&db);
    return 0;
}