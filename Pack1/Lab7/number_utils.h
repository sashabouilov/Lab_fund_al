#ifndef NUMBER_UTILS_H
#define NUMBER_UTILS_H

#include <stddef.h>

typedef enum {
    OK = 0,
    INVALID_BASE,
    INVALID_SYMBOL,
    OVERFLOW,
    ALLOCATION_ERROR,
    NULL_POINTER_ERROR
} NumberStatus;

NumberStatus min_base_for_number(const char *str, int *base);

NumberStatus str_to_ll(const char *str, int base, long long *result);

NumberStatus remove_leading_zeros(const char *str, char **result);

#endif