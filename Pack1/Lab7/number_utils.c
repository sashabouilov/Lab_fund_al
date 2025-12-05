#include "number_utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define MAX_BASE 36
#define MIN_BASE 2

static int char_to_digit(char c) {
    c = toupper(c);
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    return -1;
}

NumberStatus min_base_for_number(const char *str, int *base) {
    if (!str || !base) return NULL_POINTER_ERROR;
    
    int max_digit = 1;
    while (*str) {
        int d = char_to_digit(*str);
        if (d < 0) return INVALID_SYMBOL;
        if (d > max_digit) max_digit = d;
        str++;
    }
    
    *base = (max_digit + 1) < MIN_BASE ? MIN_BASE : (max_digit + 1);
    if (*base > MAX_BASE) return INVALID_BASE;
    
    return OK;
}

NumberStatus str_to_ll(const char *str, int base, long long *result) {
    if (!str || !result) return NULL_POINTER_ERROR;
    if (base < MIN_BASE || base > MAX_BASE) return INVALID_BASE;
    
    errno = 0;
    char *endptr;
    long long val = strtoll(str, &endptr, base);
    
    if (endptr == str) return INVALID_SYMBOL;
    if (*endptr != '\0') return INVALID_SYMBOL;
    if (errno == ERANGE) return OVERFLOW;
    
    *result = val;
    return OK;
}

NumberStatus remove_leading_zeros(const char *str, char **result) {
    if (!str || !result) return NULL_POINTER_ERROR;
    
    while (*str == '0' && *(str + 1) != '\0') {
        str++;
    }
    
    size_t len = strlen(str);
    *result = malloc(len + 1);
    if (!*result) return ALLOCATION_ERROR;
    
    strcpy(*result, str);
    return OK;
}