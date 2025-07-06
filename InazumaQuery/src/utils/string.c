#include "InazumaQuery/utils/string.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "InazumaQuery/core/errno.h"

struct ina_string_t
{
    char *buf;
};

char *alloc_string(char const *val)
{
    size_t buf_size = strlen(val) + 1;
    char *buf = malloc(buf_size);
    if (!buf)
    {
        ina_errno = INA_ERRT_STD;
        ina_stderrno = errno;
        return NULL;
    }

    strlcpy(buf, val, buf_size);

    return buf;
}

ina_string_t *ina_string_create(char const *val)
{
    ina_string_t *str = malloc(sizeof(*str));
    if (!str)
    {
        ina_errno = INA_ERRT_STD;
        ina_stderrno = errno;
        return NULL;
    }

    str->buf = alloc_string(val);
    if (!str->buf)
    {
        ina_errno = INA_ERRT_STD;
        ina_stderrno = errno;
        return NULL;
    }

    return str;
}

int ina_string_set(ina_string_t *str, char const *val)
{
    if (!str)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return 0;
    }

    if (str->buf) free(str->buf);

    str->buf = alloc_string(val);
    if (!str->buf)
    {
        ina_errno = INA_ERRT_STD;
        ina_stderrno = errno;
        return 0;
    }

    return 1;
}

int ina_string_append(ina_string_t *str, char const *val)
{
    if (!str)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return 0;
    }

    if (!str->buf)
    {
        str->buf = alloc_string(val);
        if (!str->buf)
        {
            ina_errno = INA_ERRT_STD;
            ina_stderrno = errno;
            return 0;
        }

        return 1;
    }

    size_t old_strlen = strlen(str->buf);

    size_t buf_size = old_strlen + strlen(val) + 1;

    str->buf = realloc(str->buf, buf_size);

    void *append_ptr = str->buf + old_strlen;

    strlcpy(append_ptr, val, buf_size - old_strlen);

    return 1;
}

char const *ina_string_cstr(ina_string_t *str)
{
    return str->buf;
}

size_t ina_string_len(ina_string_t *a)
{
    return strlen(a->buf);
}

int ina_string_cmp(ina_string_t *a, ina_string_t *b)
{
    return strcmp(ina_string_cstr(a), ina_string_cstr(b));
}


void ina_string_destroy(ina_string_t **str)
{
    if (!(*str)) return;

    if ((*str)->buf) free((*str)->buf);

    free((*str));
}