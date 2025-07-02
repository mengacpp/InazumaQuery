#include "inazuma/utils/list.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inazuma/core/errno.h"

#define LIST_GROW_FACTOR 3.0f

int ensure_space(ina_list_t *ls);


struct ina_list_t
{
    void *buf;
    size_t buf_size;
    size_t buf_used;

    size_t e_size;
    size_t e_count;
};

ina_list_t *ina_list_create(size_t e_size)
{
    ina_list_t *ls = malloc(sizeof(*ls));

    if (!ls)
    {
        ina_stderrno = errno;
        ina_errno = INA_ERRT_STD;
        return NULL;
    }

    ls->e_size = e_size;
    ls->e_count = 0;

    ls->buf_used = 0;
    ls->buf_size = 0;

    if (!ensure_space(ls))
    {
        return NULL;
    }

    return ls;
}

int ensure_space(ina_list_t *ls)
{
    if (!ls)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return 0;
    }

    size_t buf_free = ls->buf_size - ls->buf_used;

    if (buf_free >= ls->e_size) return 1;

    size_t new_size =
        ls->buf_size == 0
            ? ls->e_size * LIST_GROW_FACTOR
            : ls->buf_size + ls->buf_size * (1.0f / LIST_GROW_FACTOR);


    // realloc work just as malloc if ls->buf is NULL
    void *temp = realloc(ls->buf, new_size);

    if (!temp)
    {
        ina_stderrno = errno;
        ina_errno = INA_ERRT_STD;
        return 0;
    }

    ls->buf = temp;

    ls->buf_size = new_size;


    return 1;
}

int ina_list_add(ina_list_t *ls, void *e)
{
    if (!ls)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return 0;
    }

    if (!ensure_space(ls)) return 0;

    void *pos = ls->buf + (ls->e_count * ls->e_size);

    memcpy(pos, e, ls->e_size);

    ls->buf_used += ls->e_size;
    ls->e_count++;

    return 1;
}

void *ina_list_at(ina_list_t const *ls, size_t id)
{
    if (!ls)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return NULL;
    }

    if (id >= ls->e_count)
    {
        ina_errno = INA_ERRT_LIST_OOB;
        return NULL;
    }

    void *e = ls->buf + ls->e_size * id;

    return e;
}


void ina_list_destroy(ina_list_t **ls)
{
    if (!ls) return;
    if (!(*ls)) return;


    free((*ls)->buf);

    free(*ls);

    return;
}

size_t ina_list_count(ina_list_t const *ls)
{
    if (!ls)
    {
        return 0;
    }

    return ls->e_count;
}

size_t ina_list_sizeof_element(ina_list_t const *ls)
{
    if (!ls) return 0;

    return ls->e_size;
}

INA_API void ina_list_fprint(ina_list_t const *ls, FILE *out,
                             ina_list_elem_printer_t elem_printer,
                             char const *sep)
{
    fprintf(out, "[");

    for (size_t i = 0; i < ina_list_count(ls); ++i)
    {
        fflush(out);
        elem_printer(out, ina_list_at(ls, i));
        fflush(out);

        if (i < ina_list_count(ls) - 1)
        {
            fprintf(out, "%s", sep);
        }
    }

    fprintf(out, "]");
}