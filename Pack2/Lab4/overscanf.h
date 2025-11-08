#ifndef OVERSCANF_H
#define OVERSCANF_H

#include <stdio.h>
#include <stdarg.h>

typedef enum {
    OVERSCANF_SUCCESS = 0,
    OVERSCANF_EOF = -1,
    OVERSCANF_INVALID_FORMAT = -2,
    OVERSCANF_INVALID_INPUT = -3,
    OVERSCANF_MEMORY_ERROR = -4,
    OVERSCANF_INVALID_BASE = -5,
    OVERSCANF_INVALID_ROMAN = -6,
    OVERSCANF_INVALID_ZECKENDORF = -7
} overscanf_status_t;

int overfscanf(FILE *stream, const char *format, ...);
int oversscanf(const char *str, const char *format, ...);

int parse_roman_numeral(const char **str, int *result);
int parse_zeckendorf(const char **str, unsigned int *result);
int parse_custom_base(const char **str, int base, int uppercase, int *result);

#endif