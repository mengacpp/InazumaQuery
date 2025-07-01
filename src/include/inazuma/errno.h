#ifndef INAZUMA_ERROR_H_
#define INAZUMA_ERROR_H_

#include "inazuma/core.h"

#define err(x) INA_ERRT_##x
typedef enum InaErrorType
{
    err(OK) = 0,
    err(CSV_STRUCTURE_INVALID),
    err(CSV_CELL_INVALID),
    err(PARAM_NULL),
    err(PARAM_INVALID),
    err(HMAP_NOSPACE),
    err(HMAP_NOTFOUND),
    err(STDERROR),
    err(UNKNOWN)
} InaErrorType;
#undef err

extern INA_THREAD_LOCAL InaErrorType ina_errno;
extern INA_THREAD_LOCAL int ina_stderrno;

#ifdef __cplusplus
extern "C"
{
#endif
    INA_API char *ina_strerrno(InaErrorType errno);
#ifdef __cplusplus
}
#endif

#endif // INAZUMA_ERROR_H_