#include <string.h>

#include <iostream>

extern "C"
{
    void ina_normalise(char const *str, char *result, size_t len);
    int ina_normalise_strcmp(char const *str1, char const *str2);
}


int test(char const *str, char const *expected)
{
    char result[strlen(str) + 1];

    ina_normalise(str, result, strlen(str) + 1);

    if (strcmp(result, expected) != 0)
    {
        std::cerr << "'" << str << "' --> '" << result << "' (expected '"
                  << expected << "')\n";
        return 0;
    }
    return 1;
}

int main()
{
    int failures = 0;

    failures += !test("CiAo", "ciao");
    failures += !test("    CiA     o", "ciao");

    return failures == 0 ? 0 : 1;
}