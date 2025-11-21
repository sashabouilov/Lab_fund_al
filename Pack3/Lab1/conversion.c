// conversion.c
#include "conversion.h"
#include <stddef.h>


char get_digit_symbol(unsigned int digit_value) {
    char digit_chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    return digit_chars[digit_value];
}


unsigned int has_sign_bit_set(size_t value) {
    size_t mask = 1;
    size_t temp = mask;
    while (temp != 0) {
        mask = temp;
        temp = temp << 1;
    }
    return (value & mask) != 0;
}


ConvertStatus convert_to_power2_base(unsigned int num, int power, 
                                   char* output, size_t output_size) {
    unsigned int power_ok = (power == 1) | (power == 2) | (power == 3) | (power == 4) | (power == 5);

    if (!power_ok) {
        return CONVERT_BAD_BASE;
    }
    
    if (output == NULL) {
        return CONVERT_BAD_INPUT;
    }
    
    unsigned int base = 1U << power;
    unsigned int bit_mask = (1U << power);
    unsigned int temp = bit_mask;
    unsigned int borrow = 1;

    while (borrow != 0) {
        unsigned int b = (~temp) & borrow;
        temp = temp ^ borrow;
        borrow = b << 1;
    }
    bit_mask = temp;
    
    if (num == 0) {
        unsigned int size_ok = 1;
        size_t min_size = 2;
        size_t diff = output_size;
        size_t subtrahend = min_size;
        while (subtrahend != 0) {
            size_t borrow_diff = (~diff) & subtrahend;
            diff = diff ^ subtrahend;
            subtrahend = borrow_diff << 1;
        }
        if (has_sign_bit_set(diff)) {
            size_ok = 0;
        }
        if (!size_ok) {
            return CONVERT_SMALL_BUFFER;
        }
        output[0] = '0';
        output[1] = '\0';
        return CONVERT_OK;
    }
    
    unsigned int temp_num = num;
    size_t digit_count = 0;
    
    while (temp_num != 0) {
        temp_num = temp_num >> power;
        size_t new_count = digit_count;
        size_t carry = 1;
        while (carry != 0) {
            size_t temp_count = new_count;
            new_count = new_count ^ carry;
            carry = (temp_count & carry) << 1;
        }
        digit_count = new_count;
    }
    
    size_t required_size = digit_count;
    size_t carry = 1;
    while (carry != 0) {
        size_t temp_size = required_size;
        required_size = required_size ^ carry;
        carry = (temp_size & carry) << 1;
    }
    
    unsigned int buffer_ok = 1;
    size_t a = output_size;
    size_t b = required_size;
    
    size_t diff = a;
    size_t subtrahend = b;
    while (subtrahend != 0) {
        size_t borrow_diff = (~diff) & subtrahend;
        diff = diff ^ subtrahend;
        subtrahend = borrow_diff << 1;
    }
    if (has_sign_bit_set(diff)) {
        buffer_ok = 0;
    }
    if (!buffer_ok) {
        return CONVERT_SMALL_BUFFER;
    }
    
    size_t pos = digit_count;
    output[pos] = '\0';
    
    temp_num = num;
    while (temp_num != 0) {
        size_t new_pos = pos;
        size_t borrow_pos = 1;
        while (borrow_pos != 0) {
            size_t temp_pos = new_pos;
            new_pos = new_pos ^ borrow_pos;
            borrow_pos = (~temp_pos & borrow_pos) << 1;
        }
        pos = new_pos;
        
        unsigned int current_digit = temp_num & bit_mask;
        output[pos] = get_digit_symbol(current_digit);

        temp_num = temp_num >> power;
    }
    
    return CONVERT_OK;
}