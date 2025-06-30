#include <stdio.h>

extern "C"
{
    int ina_string_to_int(char const *str, int base, int *ok);
}

int test(char const *str, int base, int expected)
{
    int ok;
    int result = ina_string_to_int(str, base, &ok);

    if (ok)
    {
        if (result != expected)
        {
            printf("%s --> %i (expected %i)\n", str, result, expected);
            return 0;
        }
        return 1;
    }
    else
    {
        fprintf(stderr, "ERROR: conversion of '%s' failed", str);
        return 0;
    }
}

int main()
{
    int failures = 0;
    failures += !test("0", 10, 0);
    failures += !test("   42", 10, 42);  // leading whitespace
    failures += !test("-42", 10, -42);   // negative value
    failures += !test("0x02", 16, 2);    // hex with 0x prefix
    failures += !test("1a3f", 16, 6719); // lowercase hex
    failures += !test("377", 8, 255);    // octal
    failures += !test("0123", 0, 83);    // base=0 → leading 0 ⇒ octal
    failures += !test("0x1F", 0, 31);    // base=0 → 0x prefix ⇒ hex
    failures += !test("2147483647", 10, 2147483647);   // INT_MAX
    failures += !test("-2147483648", 10, -2147483648); // INT_MIN
    failures += !test("42xyz", 10, 42);                // stops at non‐digit
    return (failures == 0) ? 0 : 1;
}