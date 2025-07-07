#include <stdio.h>
#include <time.h>

#include "InazumaQuery/core/errno.h"
#include "InazumaQuery/inazuma.h" // IWYU pragma: keep

int main()
{
    char const *csv_path = INA_DEBUG_DATA_DIR "/moves/ie2.csv";
    clock_t start = clock();
    ina_mdb_t *db = ina_mdb_create(csv_path);
    clock_t end = clock();

    if (!db)
    {
        ina_perror("Failed to create database");
        ina_mdb_destroy(&db);
        return 1;
    }

    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to open database: %.6f seconds\n", cpu_secs);

    ina_mdb_destroy(&db);
    return 0;
}