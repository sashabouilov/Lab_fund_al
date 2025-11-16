#ifndef CONVERSION_H
#define CONVERSION_H

#include <stddef.h>

typedef enum {
    CONVERT_OK = 0,
    CONVERT_BAD_BASE = 1,
    CONVERT_BAD_INPUT = 2,
    CONVERT_SMALL_BUFFER = 3
} ConvertStatus;

ConvertStatus convert_to_power2_base(unsigned int num, int power, 
                                   char* output, size_t output_size);

char get_digit_symbol(unsigned int digit_value);

#endif