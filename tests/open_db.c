#include <stdio.h>
#include <time.h>

#include "InazumaQuery/core/errno.h"
#include "InazumaQuery/db.h"
#include "InazumaQuery/inazuma.h" // IWYU pragma: keep

int main()
{
    clock_t start = clock();
    ina_db_t *db =
        ina_db_create_from_csv(INA_DEBUG_DATA_DIR "/players/ie3.csv");
    clock_t end = clock();

    if (!db)
    {
        fprintf(stderr, "Failed to open database: %s", ina_strerrno(ina_errno));
        ina_db_close(&db);
        return 1;
    }
    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to open database: %.6f seconds\n", cpu_secs);

    ina_db_close(&db);
    return 0;
}