#ifndef INAZUMA_CORE_ERRNO_H_
#define INAZUMA_CORE_ERRNO_H_

#include "inazuma/core/core.h"

#define err(x) INA_ERRT_##x
typedef enum ina_errt_t
{
    err(OK) = 0,
    err(CSV_STRUCTURE_INVALID),
    err(CSV_CELL_INVALID),
    err(PARAM_NULL),
    err(PARAM_INVALID),
    err(HASH_MAP_NOSPACE),
    err(HASH_MAP_NOTFOUND),
    err(LIST_OOB),
    err(STD),
    err(UNKNOWN)
} ina_errt_t;
#undef err

extern INA_THREAD_LOCAL ina_errt_t ina_errno;
extern INA_THREAD_LOCAL int ina_stderrno;

INA_BEGIN_EXTERN_C

INA_API char *ina_strerrno(ina_errt_t ina_errno);

INA_END_EXTERN_C

#endif // INAZUMA_CORE_ERRNO_H_