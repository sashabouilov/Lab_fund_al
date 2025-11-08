#include "overscanf.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void skip_whitespace(const char **str) {
    if (str == NULL || *str == NULL) return;
    
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


int parse_roman_numeral(const char **str, int *result) {
    if (str == NULL || *str == NULL || result == NULL) {
        return OVERSCANF_INVALID_INPUT;
    }

    const char *ptr = *str;
    int total = 0;
    int prev_value = 0;
    int max_repeat = 0;
    char last_char = 0;
    
    skip_whitespace(&ptr);
    
    if (!*ptr || !roman_digit_value(*ptr)) {
        return OVERSCANF_INVALID_ROMAN;
    }
    
    while (*ptr && roman_digit_value(*ptr)) {
        int current_value = roman_digit_value(*ptr);
        
        if (last_char == *ptr) {
            max_repeat++;
            if (max_repeat > 3) {
                return OVERSCANF_INVALID_ROMAN;
            }
        } else {
            max_repeat = 1;
            last_char = *ptr;
        }
        
        if (prev_value < current_value) {
            if (prev_value != 0 && (current_value / prev_value > 10)) {
                return OVERSCANF_INVALID_ROMAN;
            }
            if (prev_value == 5 || prev_value == 50 || prev_value == 500) {
                return OVERSCANF_INVALID_ROMAN;
            }
            total += current_value - 2 * prev_value;
        } else {
            total += current_value;
        }
        
        prev_value = current_value;
        ptr++;
    }
    
    if (total <= 0) {
        return OVERSCANF_INVALID_ROMAN;
    }
    
    *result = total;
    *str = ptr;
    return OVERSCANF_SUCCESS;
}


int parse_zeckendorf(const char **str, unsigned int *result) {
    if (str == NULL || *str == NULL || result == NULL) {
        return OVERSCANF_INVALID_INPUT;
    }

    const char *ptr = *str;
    
    skip_whitespace(&ptr);
    
    int has_ones = 0;
    const char *temp = ptr;
    while (*temp && (*temp == '0' || *temp == '1')) {
        if (*temp == '1') has_ones = 1;
        temp++;
    }
    
    if (!has_ones) {
        return OVERSCANF_INVALID_ZECKENDORF;
    }
    
    unsigned int value = 0;
    int last_was_one = 0;
    int bit_count = 0;
    
    while (*ptr && (*ptr == '0' || *ptr == '1')) {
        if (*ptr == '1') {
            if (last_was_one) {
                return OVERSCANF_INVALID_ZECKENDORF;
            }
            value = (value << 1) | 1;
            last_was_one = 1;
        } else {
            value = value << 1;
            last_was_one = 0;
        }
        ptr++;
        bit_count++;
    }

    if (last_was_one && bit_count > 1) {
        return OVERSCANF_INVALID_ZECKENDORF;
    }
    
    *result = value;
    *str = ptr;
    return OVERSCANF_SUCCESS;
}


int parse_custom_base(const char **str, int base, int uppercase, int *result) {
    if (str == NULL || *str == NULL || result == NULL) {
        return OVERSCANF_INVALID_INPUT;
    }

    const char *ptr = *str;
    int total = 0;
    int sign = 1;
    int digit_value;
    
    skip_whitespace(&ptr);
    
    if (base < 2 || base > 36) {
        return OVERSCANF_INVALID_BASE;
    }
    
    if (*ptr == '-') {
        sign = -1;
        ptr++;
    } else if (*ptr == '+') {
        ptr++;
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
        
        if (total > (INT_MAX - digit_value) / base) {
            return OVERSCANF_INVALID_INPUT;
        }
        
        total = total * base + digit_value;
        has_digits = 1;
        ptr++;
    }
    
    if (!has_digits) {
        return OVERSCANF_INVALID_INPUT;
    }
    
    *result = sign * total;
    *str = ptr;
    return OVERSCANF_SUCCESS;
}


static int parse_standard_specifier_simple(const char **str_ptr, FILE *stream, 
                                          const char *specifier, int suppress, void *arg) {
    char format[32];
    snprintf(format, sizeof(format), "%%%s%s", suppress ? "*" : "", specifier);
    
    if (stream) {
        if (suppress) {
            return fscanf(stream, format);
        } else {
            return fscanf(stream, format, arg);
        }
    } else {
        if (suppress) {
            return sscanf(*str_ptr, format);
        } else {
            int result = sscanf(*str_ptr, format, arg);
            if (result == 1) {
                const char *temp = *str_ptr;
                skip_whitespace(&temp);
                
                if (strcmp(specifier, "s") == 0) {
                    while (*temp && !isspace((unsigned char)*temp)) {
                        temp++;
                    }
                } else if (strchr("diu", specifier[0])) {
                    if (*temp == '+' || *temp == '-') temp++;
                    while (*temp && isdigit((unsigned char)*temp)) {
                        temp++;
                    }
                } else if (strchr("feEgG", specifier[0])) {
                    if (*temp == '+' || *temp == '-') temp++;
                    while (*temp && isdigit((unsigned char)*temp)) {
                        temp++;
                    }
                    if (*temp == '.') {
                        temp++;
                        while (*temp && isdigit((unsigned char)*temp)) {
                            temp++;
                        }
                    }
                }
                
                skip_whitespace(&temp);
                *str_ptr = temp;
            }
            return result;
        }
    }
}


static int voverfscanf(FILE *stream, const char *str, const char *format, va_list args) {
    if (format == NULL) {
        return OVERSCANF_INVALID_FORMAT;
    }

    const char *fmt_ptr = format;
    const char *str_ptr = str;
    int count = 0;
    
    while (*fmt_ptr) {
        if (*fmt_ptr == '%') {
            fmt_ptr++;
            
            if (*fmt_ptr == '%') {
                if (stream) {
                    int c = fgetc(stream);
                    if (c != '%') {
                        if (c != EOF) ungetc(c, stream);
                        return count;
                    }
                } else {
                    if (*str_ptr != '%') {
                        return count;
                    }
                    str_ptr++;
                }
                fmt_ptr++;
                continue;
            }
            
            int suppress = 0;
            int width = 0;
            
            if (*fmt_ptr == '*') {
                suppress = 1;
                fmt_ptr++;
            }
            
            while (isdigit((unsigned char)*fmt_ptr)) {
                width = width * 10 + (*fmt_ptr - '0');
                fmt_ptr++;
            }
            
            if (fmt_ptr[0] == 'R' && fmt_ptr[1] == 'o') {
                fmt_ptr += 2;
                
                if (!suppress) {
                    int *value_ptr = va_arg(args, int*);
                    int value;
                    int status;
                    
                    if (stream) {
                        char buffer[256];
                        if (fscanf(stream, "%255s", buffer) != 1) {
                            return count > 0 ? count : OVERSCANF_EOF;
                        }
                        const char *buf_ptr = buffer;
                        status = parse_roman_numeral(&buf_ptr, &value);
                    } else {
                        status = parse_roman_numeral(&str_ptr, &value);
                    }
                    
                    if (status == OVERSCANF_SUCCESS) {
                        *value_ptr = value;
                        count++;
                    }
                }
                
            } else if (fmt_ptr[0] == 'Z' && fmt_ptr[1] == 'r') {
                fmt_ptr += 2;
                
                if (!suppress) {
                    unsigned int *value_ptr = va_arg(args, unsigned int*);
                    unsigned int value;
                    int status;
                    
                    if (stream) {
                        char buffer[256];
                        if (fscanf(stream, "%255s", buffer) != 1) {
                            return count > 0 ? count : OVERSCANF_EOF;
                        }
                        const char *buf_ptr = buffer;
                        status = parse_zeckendorf(&buf_ptr, &value);
                    } else {
                        status = parse_zeckendorf(&str_ptr, &value);
                    }
                    
                    if (status == OVERSCANF_SUCCESS) {
                        *value_ptr = value;
                        count++;
                    }
                }
                
            } else if (fmt_ptr[0] == 'C' && (fmt_ptr[1] == 'v' || fmt_ptr[1] == 'V')) {
                fmt_ptr += 2;
                
                if (!suppress) {
                    int *value_ptr = va_arg(args, int*);
                    int base = va_arg(args, int);
                    int uppercase_flag = (fmt_ptr[-1] == 'V');
                    int value;
                    int status;
                    
                    if (stream) {
                        char buffer[256];
                        if (fscanf(stream, "%255s", buffer) != 1) {
                            return count > 0 ? count : OVERSCANF_EOF;
                        }
                        const char *buf_ptr = buffer;
                        status = parse_custom_base(&buf_ptr, base, uppercase_flag, &value);
                    } else {
                        status = parse_custom_base(&str_ptr, base, uppercase_flag, &value);
                    }
                    
                    if (status == OVERSCANF_SUCCESS) {
                        *value_ptr = value;
                        count++;
                    }
                }
                
            } else {
                char spec_char = *fmt_ptr;
                fmt_ptr++;
                
                if (!suppress) {
                    void *arg = va_arg(args, void*);
                    char spec_str[2] = { spec_char, '\0' };
                    int result = parse_standard_specifier_simple(&str_ptr, stream, spec_str, suppress, arg);
                    
                    if (result == 1) {
                        count++;
                    } else if (result == EOF) {
                        return count > 0 ? count : OVERSCANF_EOF;
                    }
                } else {
                    char spec_str[2] = { spec_char, '\0' };
                    parse_standard_specifier_simple(&str_ptr, stream, spec_str, suppress, NULL);
                }
            }
        } else if (isspace((unsigned char)*fmt_ptr)) {
            skip_whitespace(&fmt_ptr);
            if (stream) {
                int c;
                do {
                    c = fgetc(stream);
                    if (c == EOF) return count;
                } while (isspace(c));
                ungetc(c, stream);
            } else {
                skip_whitespace(&str_ptr);
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
    if (stream == NULL || format == NULL) {
        return OVERSCANF_INVALID_INPUT;
    }

    va_list args;
    va_start(args, format);
    int result = voverfscanf(stream, NULL, format, args);
    va_end(args);
    return result;
}


int oversscanf(const char *str, const char *format, ...) {
    if (str == NULL || format == NULL) {
        return OVERSCANF_INVALID_INPUT;
    }

    va_list args;
    va_start(args, format);
    int result = voverfscanf(NULL, str, format, args);
    va_end(args);
    return result;
}
