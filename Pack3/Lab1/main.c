#include <stdio.h>
#include "conversion.h"


void show_conversion(unsigned int number, int power) {
    char result[65];
    ConvertStatus status = convert_to_power2_base(number, power, result, sizeof(result));
    
    const char* base_names[] = {"binary", "base-4", "base-8", "base-16", "base-32"};
    
    switch (status) {
        case CONVERT_OK:
            printf("Number %u in %s system (base 2^%d): %s\n", 
                   number, base_names[power-1], power, result);
            break;
        case CONVERT_BAD_BASE:
            printf("Error: invalid base power=%d (must be 1-5)\n", power);
            break;
        case CONVERT_BAD_INPUT:
            printf("Error: invalid input parameters\n");
            break;
        case CONVERT_SMALL_BUFFER:
            printf("Error: buffer too small\n");
            break;
        default:
            printf("Error: unknown error\n");
            break;
    }
}


void run_tests() {
    printf("Number System Conversion Testing\n\n");
    
    unsigned int test_nums[] = {0, 1, 5, 10, 15, 16, 31, 32, 100, 255, 1024};
    size_t test_count = sizeof(test_nums) / sizeof(test_nums[0]);
    
    for (int pwr = 1; pwr <= 5; pwr++) {
        printf("\nConversion to base 2^%d \n", pwr);
        
        for (size_t i = 0; i < test_count; i++) {
            show_conversion(test_nums[i], pwr);
        }
    }
}


void user_input_demo() {
    printf("\nConvert Your Number\n");
    unsigned int user_num;
    int user_power;
    
    printf("Enter a number in decimal system: ");
    if (scanf("%u", &user_num) == 1) {
        printf("Enter base power (1-5): ");
        if (scanf("%d", &user_power) == 1) {
            show_conversion(user_num, user_power);
        } else {
            printf("Invalid power input\n");
        }
    } else {
        printf("Invalid number input\n");
    }
}


int main() {
    run_tests();
    user_input_demo();
    
    return 0;
}