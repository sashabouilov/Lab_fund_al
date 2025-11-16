#include "conversion.h"
#include <stddef.h>

char get_digit_symbol(unsigned int digit_value) {
    return (digit_value < 10) ? 
           (char)('0' + digit_value) : 
           (char)('A' + (digit_value - 10));
}


ConvertStatus convert_to_power2_base(unsigned int num, int power, 
                                   char* output, size_t output_size) {
    if (power < 1 || power > 5) {
        return CONVERT_BAD_BASE;
    }
    
    if (output == NULL) {
        return CONVERT_BAD_INPUT;
    }
    
    unsigned int base = 1U << power;
    
    unsigned int bit_mask = 0;
    unsigned int temp_mask = 1;
    for (int i = 0; i < power; i++) {
        bit_mask = (bit_mask << 1) | temp_mask;
    }
    
    if (num == 0) {
        size_t min_size = 2;
        int buffer_ok = (output_size >= min_size);
        
        if (!buffer_ok) {
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
            size_t temp = new_count;
            new_count = new_count ^ carry;
            carry = (temp & carry) << 1;
        }
        digit_count = new_count;
    }
    
    size_t required_size = digit_count;
    size_t carry = 1;
    while (carry != 0) {
        size_t temp = required_size;
        required_size = required_size ^ carry;
        carry = (temp & carry) << 1;
    }
    
    int buffer_ok = (output_size >= required_size);
    if (!buffer_ok) {
        return CONVERT_SMALL_BUFFER;
    }
    
    size_t pos = digit_count;
    output[pos] = '\0';
    
    temp_num = num;
    while (temp_num != 0) {
        size_t new_pos = pos;
        size_t borrow = 1;
        while (borrow != 0) {
            size_t temp = new_pos;
            new_pos = new_pos ^ borrow;
            borrow = (~temp & borrow) << 1;
        }
        pos = new_pos;
        
        unsigned int current_digit = temp_num & bit_mask;
        output[pos] = get_digit_symbol(current_digit);
        
        temp_num = temp_num >> power;
    }
    
    return CONVERT_OK;
}