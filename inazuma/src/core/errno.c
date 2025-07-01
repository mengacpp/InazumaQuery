#include "inazuma/core/errno.h"

#include <string.h>

INA_THREAD_LOCAL InaErrorType ina_errno = INA_ERRT_OK;
INA_THREAD_LOCAL int ina_stderrno = 0;

#define ERRMSG(errt, msg) static char MSG_##errt[] = msg;

ERRMSG(INA_ERRT_OK, "NO ERROR")
ERRMSG(INA_ERRT_CSV_STRUCTURE_INVALID, "Invalid CSV Structure")
ERRMSG(INA_ERRT_CSV_CELL_INVALID, "Invalid CSV Cell content")
ERRMSG(INA_ERRT_PARAM_INVALID, "Invalid function parameter")
ERRMSG(INA_ERRT_PARAM_NULL, "Null function parameter")
ERRMSG(INA_ERRT_UNKNOWN, "Unknown error")
ERRMSG(INA_ERRT_HMAP_NOSPACE, "No space to add a new key in hash map")
ERRMSG(INA_ERRT_HMAP_NOTFOUND, "Key was not found in hash")

#define case(errt)                                                             \
    case errt:                                                                 \
        return MSG_##errt;
char *ina_strerrno(InaErrorType errno)
{
    switch (errno)
    {
    case INA_ERRT_STDERROR:
        return strerror(ina_stderrno);
    case(INA_ERRT_OK); 
    case(INA_ERRT_CSV_CELL_INVALID) 
    case(INA_ERRT_CSV_STRUCTURE_INVALID) 
    case(INA_ERRT_PARAM_INVALID) 
    case(INA_ERRT_PARAM_NULL) 
    case(INA_ERRT_UNKNOWN) 
    case(INA_ERRT_HMAP_NOSPACE) 
    case(INA_ERRT_HMAP_NOTFOUND) 
    }
}