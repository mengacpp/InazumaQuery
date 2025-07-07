#ifndef INAZUMAQUERY_CORE_ERRNO_H_
#define INAZUMAQUERY_CORE_ERRNO_H_

#include "InazumaQuery/core/core.h"

#include <stdarg.h>

#define err(x) INA_ERRT_##x
typedef enum ina_errt_t
{
    err(OK) = 0,
    err(PDB_CSV_INVALID_CONTENT),
    err(MDB_CSV_INVALID_CONTENT),
    err(PARAM_NULL),
    err(PARAM_INVALID),
    err(HASH_MAP_NOSPACE),
    err(HASH_MAP_NOTFOUND),
    err(LIST_OOB),
    err(STD),
    err(UNKNOWN),
    err(NOT_IMPL)
} ina_errt_t;
#undef err

extern INA_THREAD_LOCAL ina_errt_t ina_errno;
extern INA_THREAD_LOCAL int ina_stderrno;

INA_BEGIN_EXTERN_C

INA_API char *ina_strerrno(ina_errt_t ina_errno);

INA_API void ina_perror(char const *format, ...);

INA_END_EXTERN_C

#endif // INAZUMAQUERY_CORE_ERRNO_H_