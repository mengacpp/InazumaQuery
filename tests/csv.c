#include "InazumaQuery/utils/csv.h"
#include "InazumaQuery/core/errno.h"

#include <stddef.h>
#include <stdio.h>
#include <time.h>

int main()
{
    clock_t start = clock();
    ina_csv_t *csv = ina_csv_create(INA_DEBUG_DATA_DIR "/players/ie3.csv");
    clock_t end = clock();

    if (!csv)
    {
        fprintf(stderr, "ERROR: %s", ina_strerrno(ina_errno));
        return 1;
    }

    double cpu_secs = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to parse csv: %.6f seconds\n", cpu_secs);

    ina_csv_destroy(&csv);

    return 0;
}