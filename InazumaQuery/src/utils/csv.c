#include "InazumaQuery/utils/csv.h"
#include "InazumaQuery/core/errno.h"
#include "InazumaQuery/utils/list.h"
#include "InazumaQuery/utils/utils.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ina_csv_t
{
    ina_list_t *cells;
    ina_list_t *row_start_index;
    u_int32_t row_count;
};

bool parse_csv(ina_csv_t *csv, char const *content);

ina_csv_t *ina_csv_create(char const *path)
{
    ina_csv_t *csv = malloc(sizeof(*csv));

    if (!csv)
    {
        ina_errno = INA_ERRT_STD;
        ina_stderrno = errno;
        return NULL;
    }

    char *content = ina_file_readall(path);
    if (!content)
    {
        ina_csv_destroy(&csv);
        return NULL;
    }

    if (!parse_csv(csv, content))
    {
        free(content);
        ina_csv_destroy(&csv);
        return NULL;
    }

    free(content);
    return csv;
}

char const *ina_csv_get_cell(ina_csv_t *csv, uint32_t row, uint32_t col)
{
    if (!csv)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return NULL;
    }

    uint32_t *row_start = ina_list_at(csv->row_start_index, row);

    if (!row_start) return "\0";

    char const *content = *(char **)ina_list_at(csv->cells, (*row_start) + col);

    if (!content) return "\0";

    return content;
}

void ina_csv_destroy(ina_csv_t **csv)
{
    if (!(*csv)) return;

    for (size_t i = 0; i < ina_list_count((*csv)->cells); ++i)
    {
        char **buf = ina_list_at((*csv)->cells, i);

        free(*buf);
    }

    ina_list_destroy(&(*csv)->cells);
    ina_list_destroy(&(*csv)->row_start_index);

    free(*csv);
}

bool is_separator(char c)
{
    if (c == ',' || c == '\n' || c == '\0') return true;

    return false;
}

bool parse_csv(ina_csv_t *csv, char const *content)
{
    if (!csv || !content)
    {
        ina_errno = INA_ERRT_PARAM_NULL;
        return false;
    }

    csv->cells = ina_list_create(sizeof(char **));
    csv->row_start_index = ina_list_create(sizeof(uint32_t));

    int current_row = 0;
    int current_col = 0;
    uint32_t current_cell = 0;
    ina_list_add(csv->row_start_index, &current_cell);

    size_t len = strlen(content);
    for (size_t i = 0; i < len;)
    {
        size_t next_separator = i;
        while (!is_separator(content[next_separator++]))
        {
        }
        next_separator--;

        size_t cell_len = next_separator - i;
        size_t cell_start = i;
        char *buf = malloc(cell_len + 1);
        if (!buf)
        {
            ina_errno = INA_ERRT_STD;
            ina_stderrno = errno;
            return false;
        }

        memcpy(buf, content + cell_start, cell_len);
        buf[cell_len] = '\0';

        ina_list_add(csv->cells, &buf);
        current_cell++;
        i = next_separator;

        if (content[i] == ',')
        {
            current_col++;
        }
        else if (content[i] == '\n')
        {
            current_row++;
            current_col = 0;
            ina_list_add(csv->row_start_index, &current_cell);
        }
        i++;
    }


    csv->row_count = current_row + 1;
    return true;
}

uint32_t ina_csv_row_count(ina_csv_t *csv)
{
    return csv->row_count;
}