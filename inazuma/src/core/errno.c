#include "inazuma/core/errno.h"

#include <string.h>

INA_THREAD_LOCAL ina_errt_t ina_errno = INA_ERRT_OK;
INA_THREAD_LOCAL int ina_stderrno = 0;

#define ERRMSG(errt, msg) static char MSG_##errt[] = msg;

ERRMSG(INA_ERRT_OK, "Success")
ERRMSG(INA_ERRT_CSV_STRUCTURE_INVALID, "CSV parse error: invalid structure")
ERRMSG(INA_ERRT_CSV_CELL_INVALID, "CSV parse error: invalid cell content")
ERRMSG(INA_ERRT_PARAM_INVALID, "Invalid parameter")
ERRMSG(INA_ERRT_PARAM_NULL, "Null pointer passed as parameter")
ERRMSG(INA_ERRT_UNKNOWN, "An unknown error occurred")
ERRMSG(INA_ERRT_HASH_MAP_NOSPACE, "Hash map full: cannot add key")
ERRMSG(INA_ERRT_HASH_MAP_NOTFOUND, "Hash map lookup failed: key not found")
ERRMSG(INA_ERRT_LIST_OOB, "List index out of bounds")

#define case(errt)                                                             \
    case errt:                                                                 \
        return MSG_##errt;

char *ina_strerrno(ina_errt_t ina_errno)
{
    switch (ina_errno)
    {
    case INA_ERRT_STD:
        return strerror(ina_stderrno);
    case(INA_ERRT_OK); 
    case(INA_ERRT_CSV_CELL_INVALID) 
    case(INA_ERRT_CSV_STRUCTURE_INVALID) 
    case(INA_ERRT_PARAM_INVALID) 
    case(INA_ERRT_PARAM_NULL) 
    case(INA_ERRT_UNKNOWN) 
    case(INA_ERRT_HASH_MAP_NOSPACE) 
    case(INA_ERRT_HASH_MAP_NOTFOUND) 
    case(INA_ERRT_LIST_OOB)
    }
}