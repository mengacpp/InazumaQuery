#include "inazuma/core/errno.h"
#include "inazuma/inazuma.h" // IWYU pragma: keep

#include "stdio.h"
#include <stdio.h>

int main()
{
    // when a function fails, the ina_errno variable is set.
    // if ina_errno == INA_ERRT_STDERROR, then also ina_stderrno is set
    // to indicate the stdlib error that occurred
    InaPlayerDB *db = ina_player_db_open("invalid/path/to/csv");

    if (!db)
    {
        // ina_strerrno(ina_errno) will print the correct error message even if
        // a stdlib error occurred
        fprintf(stderr, "ERROR: database not opened: %s\n",
                ina_strerrno(ina_errno));
    }
}