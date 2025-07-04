#include "inazuma/utils/utils.h"

#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

#include "inazuma/core/errno.h"


char *ina_read_file_content(const char *path_to_file)
{
    FILE *fp = fopen(path_to_file, "rb");
    if (!fp)
    {
        ina_errno = INA_ERRT_STD;
        ina_stderrno = errno;
        return NULL;
    }

    if (fseek(fp, 0, SEEK_END) != 0)
    {
        fclose(fp);
        ina_errno = INA_ERRT_STD;
        ina_stderrno = errno;
        return NULL;
    }

    long filesize = ftell(fp);
    if (filesize < 0)
    {
        fclose(fp);
        ina_errno = INA_ERRT_STD;
        ina_stderrno = errno;
        return NULL;
    }

    rewind(fp);

    char *buffer = (char *)malloc((size_t)filesize + 1);
    if (!buffer)
    {
        fclose(fp);
        ina_errno = INA_ERRT_STD;
        ina_stderrno = ENOMEM;
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, (size_t)filesize, fp);
    fclose(fp);

    if (bytes_read != (size_t)filesize)
    {
        free(buffer);
        ina_errno = INA_ERRT_STD;
        ina_stderrno = errno;
        return NULL;
    }

    buffer[bytes_read] = '\0';
    return buffer;
}

void ina_normalise(char const *str, char *result, size_t len)
{
    size_t current = 0;

    // Process each character until '\0', skipping spaces
    for (size_t i = 0; str[i] != '\0' && current + 1 < len; ++i)
    {
        unsigned char c = (unsigned char)str[i];
        if (isspace(c)) continue;
        result[current++] = (char)tolower(c);
    }

    // Always terminate within bounds
    result[current] = '\0';
}

int ina_normalise_strcmp(char const *a, char const *b)
{
    size_t na = strlen(a) + 1;
    size_t nb = strlen(b) + 1;
    char norm1[na], norm2[nb];

    ina_normalise(a, norm1, na);
    ina_normalise(b, norm2, nb);

    return strcmp(norm1, norm2);
}

void ina_retrieve_digits(char const *str, char *result, size_t len)
{
    if (len == 0)
    {
        return;
    }
    else if (strcmp(str, "\0") == 0 || len == 1)
    {
        result[0] = '\0';
        return;
    }

    size_t used_space = 0;

    for (size_t i = 0; i < strlen(str); ++i)
    {
        if (isdigit(str[i]))
        {
            result[used_space] = str[i];

            used_space++;

            if (used_space >= len - 1)
            {
                return;
            }
        }
    }

    result[used_space] = '\0';
}

int ina_string_to_int(char const *str, int base)
{
    char *end;
    errno = 0;
    long val = strtol(str, &end, base);

    if (errno != 0)
    {
        return 0;
    }

    return val;
}
