#include "InazumaQuery/utils/string.h"
#include "InazumaQuery/core/errno.h"
#include <stdio.h>

int main()
{
    ina_string_t *str = ina_string_create("ciao");
    if (!str)
    {
        fprintf(stderr, "%s\n", ina_strerrno(ina_errno));
        return 1;
    }

    printf("%s\n", ina_string_cstr(str));

    ina_string_append(str, " i just appended this");

    printf("%s\n", ina_string_cstr(str));

    ina_string_set(str, "changed the string to this");

    printf("%s\n", ina_string_cstr(str));

    ina_string_destroy(&str);

    return 0;
}