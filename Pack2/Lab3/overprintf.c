#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include "overprintf.h"

#define BUFFER_SIZE 1024


overprintf_status_t int_to_roman(int value, char *buffer, size_t buffer_size) {
    if (value <= 0 || value > 3999) {
        return OVERPRINTF_INVALID_ROMAN;
    }
    if (!buffer || buffer_size == 0) {
        return OVERPRINTF_NULL_POINTER;
    }
    
    static const struct {
        int value;
        const char *numeral;
    } romans[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };
    
    size_t pos = 0;
    int temp = value;
    
    for (size_t i = 0; i < sizeof(romans) / sizeof(romans[0]); i++) {
        while (temp >= romans[i].value) {
            const char *numeral = romans[i].numeral;
            size_t len = strlen(numeral);
            if (pos + len + 1 > buffer_size) {
                buffer[0] = '\0';
                return OVERPRINTF_BUFFER_OVERFLOW;
            }
            strcpy(buffer + pos, numeral);
            pos += len;
            temp -= romans[i].value;
        }
    }
    
    buffer[pos] = '\0';
    return OVERPRINTF_SUCCESS;
}


overprintf_status_t uint_to_zeckendorf(unsigned int value, char *buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) {
        return OVERPRINTF_NULL_POINTER;
    }
    if (value == 0) {
        if (buffer_size < 2) {
            buffer[0] = '\0';
            return OVERPRINTF_BUFFER_OVERFLOW;
        }
        strcpy(buffer, "0");
        return OVERPRINTF_SUCCESS;
    }
    
    unsigned int fib[50];
    int fib_count = 0;
    
    fib[0] = 1;
    fib[1] = 2;
    fib_count = 2;
    
    for (int i = 2; i < 50; i++) {
        fib[i] = fib[i-1] + fib[i-2];
        if (fib[i] > value) {
            break;
        }
        fib_count++;
    }
    
    size_t pos = 0;
    unsigned int temp = value;
    
    char temp_buf[50];
    int temp_pos = 0;
    
    for (int i = fib_count - 1; i >= 0; i--) {
        if (temp >= fib[i]) {
            if (temp_pos >= 49) {
                buffer[0] = '\0';
                return OVERPRINTF_BUFFER_OVERFLOW;
            }
            temp_buf[temp_pos++] = '1';
            temp -= fib[i];
        } else {
            if (temp_pos >= 49) {
                buffer[0] = '\0';
                return OVERPRINTF_BUFFER_OVERFLOW;
            }
            temp_buf[temp_pos++] = '0';
        }
    }
    
    for (int i = temp_pos - 1; i >= 0; i--) {
        if (pos + 2 > buffer_size) {
            buffer[0] = '\0';
            return OVERPRINTF_BUFFER_OVERFLOW;
        }
        buffer[pos++] = temp_buf[i];
    }
    
    if (pos + 2 > buffer_size) {
        buffer[0] = '\0';
        return OVERPRINTF_BUFFER_OVERFLOW;
    }
    buffer[pos++] = '1';
    buffer[pos] = '\0';
    
    return OVERPRINTF_SUCCESS;
}


overprintf_status_t int_to_custom_base(int value, int base, int uppercase, char *buffer, size_t buffer_size) {
    if (base < 2 || base > 36) {
        base = 10;
    }
    if (!buffer || buffer_size == 0) {
        return OVERPRINTF_NULL_POINTER;
    }
    
    int is_negative = value < 0;
    unsigned int abs_value = is_negative ? (unsigned int)(-value) : (unsigned int)value;
    char temp[65];
    int pos = sizeof(temp) - 1;
    temp[pos] = '\0';
    
    if (abs_value == 0) {
        temp[--pos] = '0';
    } else {
        while (abs_value > 0) {
            int digit = abs_value % base;
            char c;
            
            if (digit < 10) {
                c = '0' + digit;
            } else {
                c = (uppercase ? 'A' : 'a') + (digit - 10);
            }
            
            temp[--pos] = c;
            abs_value /= base;
        }
    }
    
    if (is_negative) {
        temp[--pos] = '-';
    }
    
    size_t len = sizeof(temp) - pos;
    if (len >= buffer_size) {
        return OVERPRINTF_BUFFER_OVERFLOW;
    }
    
    strcpy(buffer, temp + pos);
    return OVERPRINTF_SUCCESS;
}


overprintf_status_t string_to_int_base(const char *str, int base, int uppercase, int *result) {
    if (!str || !result) {
        return OVERPRINTF_NULL_POINTER;
    }
    if (base < 2 || base > 36) {
        base = 10;
    }

    while (isspace((unsigned char)*str)) {
        str++;
    }
    
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }
    
    long value = 0;
    int valid_digits = 0;
    
    while (*str) {
        char c = *str;
        int digit;
        
        if (isdigit((unsigned char)c)) {
            digit = c - '0';
        } else if (isalpha((unsigned char)c)) {
            if (uppercase) {
                digit = toupper(c) - 'A' + 10;
            } else {
                digit = tolower(c) - 'a' + 10;
            }
        } else {
            break;
        }
        
        if (digit >= base) {
            break;
        }
        
        if (value > (LONG_MAX - digit) / base) {
            return OVERPRINTF_INVALID_STRING;
        }
        
        value = value * base + digit;
        valid_digits++;
        str++;
    }
    
    if (valid_digits == 0) {
        return OVERPRINTF_INVALID_STRING;
    }
    
    if (value > INT_MAX || value < INT_MIN) {
        return OVERPRINTF_INVALID_STRING;
    }
    
    *result = (int)(value * sign);
    return OVERPRINTF_SUCCESS;
}


overprintf_status_t memory_dump(const void *ptr, size_t size, char *buffer, size_t buffer_size) {
    if (!ptr || !buffer) {
        return OVERPRINTF_NULL_POINTER;
    }
    
    const unsigned char *bytes = (const unsigned char *)ptr;
    size_t needed_size = size * 9;
    
    if (buffer_size < needed_size) {
        return OVERPRINTF_BUFFER_OVERFLOW;
    }
    
    size_t pos = 0;
    for (size_t i = 0; i < size; i++) {
        for (int bit = 7; bit >= 0; bit--) {
            buffer[pos++] = (bytes[i] & (1 << bit)) ? '1' : '0';
        }
        if (i < size - 1) {
            buffer[pos++] = ' ';
        }
    }
    buffer[pos] = '\0';
    return OVERPRINTF_SUCCESS;
}


static overprintf_status_t process_specifier(const char **format, va_list *args, 
                                           char *output, size_t output_size) {
    const char *fmt = *format;
    
    fmt++;
    
    if (!*fmt) {
        return OVERPRINTF_INVALID_FORMAT;
    }
    
    if (strchr("diuoxXfcsSp%", *fmt)) {
        char specifier = *fmt;
        (*format) += 2;
        
        char temp[256];
        
        switch (specifier) {
            case 'd':
            case 'i': {
                int value = va_arg(*args, int);
                snprintf(temp, sizeof(temp), "%d", value);
                break;
            }
            case 'u': {
                unsigned int value = va_arg(*args, unsigned int);
                snprintf(temp, sizeof(temp), "%u", value);
                break;
            }
            case 'x': {
                unsigned int value = va_arg(*args, unsigned int);
                snprintf(temp, sizeof(temp), "%x", value);
                break;
            }
            case 'X': {
                unsigned int value = va_arg(*args, unsigned int);
                snprintf(temp, sizeof(temp), "%X", value);
                break;
            }
            case 'o': {
                unsigned int value = va_arg(*args, unsigned int);
                snprintf(temp, sizeof(temp), "%o", value);
                break;
            }
            case 'f': {
                double value = va_arg(*args, double);
                snprintf(temp, sizeof(temp), "%f", value);
                break;
            }
            case 'c': {
                char value = (char)va_arg(*args, int);
                temp[0] = value;
                temp[1] = '\0';
                break;
            }
            case 's': {
                const char *value = va_arg(*args, const char *);
                if (!value) {
                    strcpy(temp, "(null)");
                } else {
                    strncpy(temp, value, sizeof(temp) - 1);
                    temp[sizeof(temp) - 1] = '\0';
                }
                break;
            }
            case 'p': {
                void *value = va_arg(*args, void *);
                snprintf(temp, sizeof(temp), "%p", value);
                break;
            }
            case '%': {
                strcpy(temp, "%");
                break;
            }
            default:
                return OVERPRINTF_INVALID_FORMAT;
        }
        
        strncpy(output, temp, output_size - 1);
        output[output_size - 1] = '\0';
        return OVERPRINTF_SUCCESS;
    }
    
    if (fmt[0] && fmt[1]) {
        if (fmt[0] == 'R' && fmt[1] == 'o') {
            *format += 3;
            int value = va_arg(*args, int);
            return int_to_roman(value, output, output_size);
        }
        else if (fmt[0] == 'Z' && fmt[1] == 'r') {
            *format += 3;
            unsigned int value = va_arg(*args, unsigned int);
            return uint_to_zeckendorf(value, output, output_size);
        }
        else if (fmt[0] == 'C' && fmt[1] == 'v') {
            *format += 3;
            int value = va_arg(*args, int);
            int base = va_arg(*args, int);
            return int_to_custom_base(value, base, 0, output, output_size);
        }
        else if (fmt[0] == 'C' && fmt[1] == 'V') {
            *format += 3;
            int value = va_arg(*args, int);
            int base = va_arg(*args, int);
            return int_to_custom_base(value, base, 1, output, output_size);
        }
        else if (fmt[0] == 't' && fmt[1] == 'o') {
            *format += 3;
            const char *str = va_arg(*args, const char *);
            int base = va_arg(*args, int);
            int result;
            overprintf_status_t status = string_to_int_base(str, base, 0, &result);
            if (status == OVERPRINTF_SUCCESS) {
                snprintf(output, output_size, "%d", result);
            }
            return status;
        }
        else if (fmt[0] == 't' && fmt[1] == 'O') {
            *format += 3;
            const char *str = va_arg(*args, const char *);
            int base = va_arg(*args, int);
            int result;
            overprintf_status_t status = string_to_int_base(str, base, 1, &result);
            if (status == OVERPRINTF_SUCCESS) {
                snprintf(output, output_size, "%d", result);
            }
            return status;
        }
        else if (fmt[0] == 'm' && fmt[1] == 'i') {
            *format += 3;
            int value = va_arg(*args, int);
            return memory_dump(&value, sizeof(int), output, output_size);
        }
        else if (fmt[0] == 'm' && fmt[1] == 'u') {
            *format += 3;
            unsigned int value = va_arg(*args, unsigned int);
            return memory_dump(&value, sizeof(unsigned int), output, output_size);
        }
        else if (fmt[0] == 'm' && fmt[1] == 'd') {
            *format += 3;
            double value = va_arg(*args, double);
            return memory_dump(&value, sizeof(double), output, output_size);
        }
        else if (fmt[0] == 'm' && fmt[1] == 'f') {
            *format += 3;
            float value = va_arg(*args, double);
            return memory_dump(&value, sizeof(float), output, output_size);
        }
    }
    return OVERPRINTF_INVALID_FORMAT;
}


int overfprintf(FILE *stream, const char *format, ...) {
    if (!stream || !format) {
        return -1;
    }
    
    va_list args;
    va_start(args, format);
    
    int total_written = 0;
    const char *current = format;
    
    while (*current) {
        if (*current != '%') {
            fputc(*current, stream);
            total_written++;
            current++;
            continue;
        }
        
        const char *spec_start = current;
        char spec_buffer[256];
        overprintf_status_t status = process_specifier(&current, &args, spec_buffer, sizeof(spec_buffer));
        
        if (status == OVERPRINTF_SUCCESS) {
            fputs(spec_buffer, stream);
            total_written += strlen(spec_buffer);
        } else {
            fputc('%', stream);
            total_written++;
            current = spec_start + 1;
        }
    }
    
    va_end(args);
    return total_written;
}


int oversprintf(char *str, const char *format, ...) {
    if (!str || !format) {
        return -1;
    }
    
    va_list args;
    va_start(args, format);
    
    char buffer[BUFFER_SIZE];
    size_t pos = 0;
    const char *current = format;
    
    while (*current && pos < sizeof(buffer) - 1) {
        if (*current != '%') {
            buffer[pos++] = *current++;
            continue;
        }
        
        const char *spec_start = current;
        char spec_buffer[256];
        overprintf_status_t status = process_specifier(&current, &args, spec_buffer, sizeof(spec_buffer));
        
        if (status == OVERPRINTF_SUCCESS) {
            size_t spec_len = strlen(spec_buffer);
            if (pos + spec_len < sizeof(buffer)) {
                strcpy(buffer + pos, spec_buffer);
                pos += spec_len;
            } else {
                break;
            }
        } else {
            if (pos < sizeof(buffer) - 1) {
                buffer[pos++] = '%';
                current = spec_start + 1;
            } else {
                break;
            }
        }
    }
    
    buffer[pos] = '\0';
    strcpy(str, buffer);
    va_end(args);
    return (int)pos;
}