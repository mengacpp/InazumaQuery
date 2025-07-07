#include "InazumaQuery/core/errno.h"
#include "InazumaQuery/inazuma.h" // IWYU pragma: keep

#include "stdio.h"
#include <stdio.h>

int main()
{
    // when a function fails, the ina_errno variable is set.
    // if ina_errno == INA_ERRT_STD, then also ina_stderrno is set
    // to indicate the stdlib error that occurred
    ina_pdb_t *db = ina_pdb_create("invalid/path/to/csv");

    if (!db)
    {
        // ina_strerrno(ina_errno) will print the correct error message even if
        // a stdlib error occurred
        fprintf(stderr, "ERROR: database not opened: %s\n",
                ina_strerrno(ina_errno));
        return 1;
    }

    ina_pdb_destroy(&db);

    return 0;
}

// output ERROR: database not opened: no such file or directory
