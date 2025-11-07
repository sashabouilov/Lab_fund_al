#include <stdio.h>
#include <stdlib.h>
#include "overscanf.h"


void test_roman_numerals(void) {
    printf("Testing Roman Numerals (%%Ro)\n");
    
    const char *test_cases[] = {
        "III", "IV", "IX", "XIV", "XIX", "XL", "XC", "CD", "CM", "MMXXIV"
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        int value;
        int result = oversscanf(test_cases[i], "%Ro", &value);
        printf("Roman: %-8s -> Arabic: %d (result: %d)\n", test_cases[i], value, result);
    }
    printf("\n");
}


void test_zeckendorf(void) {
    printf("Testing Zeckendorf Representation (%%Zr)\n");
    
    struct {
        const char *representation;
        unsigned int expected;
    } test_cases[] = {
        {"101", 4},
        {"1001", 6},
        {"10001", 9},
        {"10100", 7},
        {"100001", 12},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        unsigned int value;
        int result = oversscanf(test_cases[i].representation, "%Zr", &value);
        printf("Zeckendorf: %-8s -> Expected: %2u, Got: %2u %s (result: %d)\n", 
               test_cases[i].representation, test_cases[i].expected, value,
               (value == test_cases[i].expected) ? "+" : "-", result);
    }
    printf("\n");
}


void test_custom_base(void) {
    printf("Testing Custom Base (%%Cv/%%CV)\n");
    
    const char *lower_case = "1af";
    int value_lower;
    int base_lower = 16;
    int result_lower = oversscanf(lower_case, "%Cv", &value_lower, base_lower);
    printf("Lowercase: %s (base %d) -> %d (result: %d)\n", 
           lower_case, base_lower, value_lower, result_lower);
    
    const char *upper_case = "1AF";
    int value_upper;
    int base_upper = 16;
    int result_upper = oversscanf(upper_case, "%CV", &value_upper, base_upper);
    printf("Uppercase: %s (base %d) -> %d (result: %d)\n", 
           upper_case, base_upper, value_upper, result_upper);
    
    const char *binary = "1101";
    int value_binary;
    int base_binary = 2;
    int result_binary = oversscanf(binary, "%Cv", &value_binary, base_binary);
    printf("Binary: %s (base %d) -> %d (result: %d)\n", 
           binary, base_binary, value_binary, result_binary);
    
    printf("\n");
}


void test_mixed_formats(void) {
    printf("Testing Mixed Formats\n");
    
    const char *mixed_input = "XIV 101 1a 42";
    int roman_val, custom_val, decimal_val;
    unsigned int zeck_val;
    
    int result = oversscanf(mixed_input, "%Ro %Zr %Cv %d", 
                           &roman_val, &zeck_val, &custom_val, 16, &decimal_val);
    
    printf("Input: \"%s\"\n", mixed_input);
    printf("Parsed: Roman=%d, Zeckendorf=%u, Custom(16)=%d, Decimal=%d (result: %d)\n",
           roman_val, zeck_val, custom_val, decimal_val, result);
    printf("\n");
}


int main(void) {
    printf("Overfscanf/OverSscanf Demonstration\n\n");
    
    test_roman_numerals();
    test_zeckendorf();
    test_custom_base();
    test_mixed_formats();
    
    printf("Testing File Input\n");
    
    FILE *test_file = fopen("test_input.txt", "w");
    if (test_file) {
        fprintf(test_file, "XXV 1001 1f 100\n");
        fclose(test_file);
    }
    
    test_file = fopen("test_input.txt", "r");
    if (test_file) {
        int roman_file, custom_file, decimal_file;
        unsigned int zeck_file;
        
        int file_result = overfscanf(test_file, "%Ro %Zr %Cv %d", 
                                    &roman_file, &zeck_file, &custom_file, 16, &decimal_file);
        
        printf("File input parsed: Roman=%d, Zeckendorf=%u, Custom(16)=%d, Decimal=%d (result: %d)\n",
               roman_file, zeck_file, custom_file, decimal_file, file_result);
        
        fclose(test_file);
    }
    
    return 0;
}