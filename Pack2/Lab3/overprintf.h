#ifndef OVERPRINTF_H
#define OVERPRINTF_H

#include <stdio.h>

typedef enum {
    OVERPRINTF_SUCCESS = 0,
    OVERPRINTF_INVALID_FORMAT,
    OVERPRINTF_INVALID_BASE,
    OVERPRINTF_INVALID_ROMAN,
    OVERPRINTF_INVALID_STRING,
    OVERPRINTF_BUFFER_OVERFLOW,
    OVERPRINTF_NULL_POINTER
} overprintf_status_t;

int overfprintf(FILE *stream, const char *format, ...);
int oversprintf(char *str, const char *format, ...);

overprintf_status_t int_to_roman(int value, char *buffer, size_t buffer_size);
overprintf_status_t uint_to_zeckendorf(unsigned int value, char *buffer, size_t buffer_size);
overprintf_status_t int_to_custom_base(int value, int base, int uppercase, char *buffer, size_t buffer_size);
overprintf_status_t string_to_int_base(const char *str, int base, int uppercase, int *result);
overprintf_status_t memory_dump(const void *ptr, size_t size, char *buffer, size_t buffer_size);

#endif