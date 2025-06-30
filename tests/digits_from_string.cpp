#include <iostream>
#include <stddef.h>
#include <string.h>

static int test_count = 0;

extern "C"
{
    void ina_retrieve_digits(char const *str, char *result, size_t len);
    int ina_string_to_int(char const *str, int base, int *ok);
}

int test(char const *str, char const *expected)
{
    char result[strlen(str) + 1];

    ina_retrieve_digits(str, result, strlen(str) + 1);

    int ok;
    int int_result = ina_string_to_int(result, 10, &ok);
    if (!ok) return 0;
    int int_expected = ina_string_to_int(expected, 10, &ok);
    if (!ok) return 0;

    if (int_result != int_expected)
    {
        std::cerr << test_count << ". '" << str << "' --> '" << result
                  << "' (expected '" << expected << "')\n";

        return 0;
    }

    test_count++;
    return 1;
}


int main()
{
    int failures = 0;
    /* empty string → empty result */
    failures += !test("", "\0");

    /* no digits at all */
    failures += !test("abcdef", "\0");

    /* only digits */
    failures += !test("12345", "12345");

    /* digits at start, middle, end */
    failures += !test("007bond", "007");
    failures += !test("a1b2c3", "123");
    failures += !test("ci2a3o", "23");

    /* interspersed with symbols/whitespace */
    failures += !test("#1#2#3!", "123");
    failures += !test(" 4 5\t6\n", "456");

    /* long run of digits */
    failures += !test("0123456789", "0123456789");

    return (failures == 0) ? 0 : 1;
}