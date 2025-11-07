#include "overscanf.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


static void skip_whitespace(const char **str) {
    while (**str && isspace((unsigned char)**str)) {
        (*str)++;
    }
}


static int roman_digit_value(char c) {
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}


int parse_roman_numeral(const char **str) {
    const char *ptr = *str;
    int result = 0;
    int prev_value = 0;
    
    skip_whitespace(&ptr);
    
    if (!roman_digit_value(*ptr)) {
        return -1;
    }
    
    while (*ptr && roman_digit_value(*ptr)) {
        int current_value = roman_digit_value(*ptr);
        
        if (prev_value < current_value) {
            result += current_value - 2 * prev_value;
        } else {
            result += current_value;
        }
        
        prev_value = current_value;
        ptr++;
    }
    
    *str = ptr;
    return result;
}


unsigned int parse_zeckendorf(const char **str) {
    const char *ptr = *str;
    unsigned int result = 0;
    
    skip_whitespace(&ptr);
    
    unsigned int fib[50];
    fib[0] = 1;
    fib[1] = 2;
    for (int i = 2; i < 50; i++) {
        fib[i] = fib[i-1] + fib[i-2];
        if (fib[i] < fib[i-1]) break;
    }
    
    int fib_index = 0;
    int last_bit_was_one = 0;
    int has_ones = 0;
    
    while (*ptr && (*ptr == '0' || *ptr == '1')) {
        if (*ptr == '1') {
            if (last_bit_was_one) {
                return (unsigned int)-1;
            }
            
            if (fib_index < 50) {
                result += fib[fib_index];
            }
            last_bit_was_one = 1;
            has_ones = 1;
        } else {
            last_bit_was_one = 0;
        }
        
        ptr++;
        fib_index++;
    }
    
    if (!has_ones || last_bit_was_one) {
        return (unsigned int)-1;
    }
    
    *str = ptr;
    return result;
}


int parse_custom_base(const char **str, int base, int uppercase) {
    const char *ptr = *str;
    int result = 0;
    int sign = 1;
    int digit_value;
    
    skip_whitespace(&ptr);
    
    if (*ptr == '-') {
        sign = -1;
        ptr++;
    } else if (*ptr == '+') {
        ptr++;
    }
    
    if (base < 2 || base > 36) {
        base = 10;
    }
    
    int has_digits = 0;
    while (*ptr) {
        char c = *ptr;
        
        if (isdigit((unsigned char)c)) {
            digit_value = c - '0';
        } else if (isalpha((unsigned char)c)) {
            if (uppercase) {
                digit_value = toupper((unsigned char)c) - 'A' + 10;
            } else {
                digit_value = tolower((unsigned char)c) - 'a' + 10;
            }
        } else {
            break;
        }
        
        if (digit_value >= base) {
            break;
        }
        
        result = result * base + digit_value;
        has_digits = 1;
        ptr++;
    }
    
    if (!has_digits) {
        return 0;
    }
    
    *str = ptr;
    return sign * result;
}


static int parse_standard_format(const char **str, FILE *stream, const char *format, va_list args) {
    if (stream) {
        return fscanf(stream, format, va_arg(args, void*));
    } else {
        return sscanf(*str, format, va_arg(args, void*));
    }
}


static int voverfscanf(FILE *stream, const char *str, const char *format, va_list args) {
    const char *fmt_ptr = format;
    const char *str_ptr = str;
    int count = 0;
    va_list args_copy;
    
    while (*fmt_ptr) {
        if (*fmt_ptr == '%') {
            fmt_ptr++;
            
            while (*fmt_ptr && strchr(" *", *fmt_ptr)) {
                if (*fmt_ptr == '*') {
                    fmt_ptr++;
                    break;
                }
                fmt_ptr++;
            }
            
            if (fmt_ptr[0] == 'R' && fmt_ptr[1] == 'o') {
                int *value_ptr = va_arg(args, int*);
                int value;
                
                if (stream) {
                    char buffer[256];
                    if (fscanf(stream, "%255s", buffer) != 1) {
                        return count > 0 ? count : OVERSCANF_EOF;
                    }
                    const char *buf_ptr = buffer;
                    value = parse_roman_numeral(&buf_ptr);
                } else {
                    value = parse_roman_numeral(&str_ptr);
                }
                
                if (value >= 0) {
                    *value_ptr = value;
                    count++;
                }
                fmt_ptr += 2;
                
            } else if (fmt_ptr[0] == 'Z' && fmt_ptr[1] == 'r') {
                unsigned int *value_ptr = va_arg(args, unsigned int*);
                unsigned int value;
                
                if (stream) {
                    char buffer[256];
                    if (fscanf(stream, "%255s", buffer) != 1) {
                        return count > 0 ? count : OVERSCANF_EOF;
                    }
                    const char *buf_ptr = buffer;
                    value = parse_zeckendorf(&buf_ptr);
                } else {
                    value = parse_zeckendorf(&str_ptr);
                }
                
                if (value != (unsigned int)-1) {
                    *value_ptr = value;
                    count++;
                }
                fmt_ptr += 2;
                
            } else if (fmt_ptr[0] == 'C' && (fmt_ptr[1] == 'v' || fmt_ptr[1] == 'V')) {
                int *value_ptr = va_arg(args, int*);
                int base = va_arg(args, int);
                int uppercase = (fmt_ptr[1] == 'V');
                int value;
                
                if (stream) {
                    char buffer[256];
                    if (fscanf(stream, "%255s", buffer) != 1) {
                        return count > 0 ? count : OVERSCANF_EOF;
                    }
                    const char *buf_ptr = buffer;
                    value = parse_custom_base(&buf_ptr, base, uppercase);
                } else {
                    value = parse_custom_base(&str_ptr, base, uppercase);
                }
                
                *value_ptr = value;
                count++;
                fmt_ptr += 2;
                
            } else {
                char specifier[8] = { '%' };
                int spec_len = 1;
                
                while (*fmt_ptr && !isalpha((unsigned char)*fmt_ptr)) {
                    if (spec_len < 6) {
                        specifier[spec_len++] = *fmt_ptr;
                    }
                    fmt_ptr++;
                }
                
                if (*fmt_ptr) {
                    specifier[spec_len++] = *fmt_ptr;
                    specifier[spec_len] = '\0';
                    fmt_ptr++;
                    
                    va_copy(args_copy, args);
                    int scan_result = parse_standard_format(&str_ptr, stream, specifier, args_copy);
                    va_end(args_copy);
                    
                    if (scan_result == 1) {
                        count++;
                        if (!stream && str_ptr) {
                            while (*str_ptr && !isspace((unsigned char)*str_ptr)) {
                                str_ptr++;
                            }
                            skip_whitespace(&str_ptr);
                        }
                    } else if (scan_result == EOF) {
                        return count > 0 ? count : OVERSCANF_EOF;
                    }
                }
            }
        } else {
            if (stream) {
                int c = fgetc(stream);
                if (c != *fmt_ptr) {
                    if (c != EOF) ungetc(c, stream);
                    return count;
                }
            } else {
                if (*str_ptr != *fmt_ptr) {
                    return count;
                }
                str_ptr++;
            }
            fmt_ptr++;
        }
    }
    
    return count;
}


int overfscanf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = voverfscanf(stream, NULL, format, args);
    va_end(args);
    return result;
}


int oversscanf(const char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = voverfscanf(NULL, str, format, args);
    va_end(args);
    return result;
}