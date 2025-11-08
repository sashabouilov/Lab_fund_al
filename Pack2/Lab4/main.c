#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "overscanf.h"


void test_roman_numerals(void) {
    printf("Testing Roman Numerals (%%Ro)\n");
    
    const char *test_cases[] = {
        "III", "IV", "IX", "XIV", "XIX", "XL", "XC", "CD", "CM", "MMXXIV"
    };
    int expected[] = {
        3, 4, 9, 14, 19, 40, 90, 400, 900, 2024
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        int value;
        int result = oversscanf(test_cases[i], "%Ro", &value);
        printf("Roman: %-8s -> Arabic: %d (expected: %d) %s (result: %d)\n", 
               test_cases[i], value, expected[i],
               (value == expected[i]) ? "+" : "-", result);
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
    printf("Lowercase: %s (base %d) -> %d (expected: 431) %s (result: %d)\n", 
           lower_case, base_lower, value_lower,
           (value_lower == 431) ? "+" : "-", result_lower);
    
    const char *upper_case = "1AF";
    int value_upper;
    int base_upper = 16;
    int result_upper = oversscanf(upper_case, "%CV", &value_upper, base_upper);
    printf("Uppercase: %s (base %d) -> %d (expected: 431) %s (result: %d)\n", 
           upper_case, base_upper, value_upper,
           (value_upper == 431) ? "+" : "-", result_upper);
    
    const char *binary = "1101";
    int value_binary;
    int base_binary = 2;
    int result_binary = oversscanf(binary, "%Cv", &value_binary, base_binary);
    printf("Binary: %s (base %d) -> %d (expected: 13) %s (result: %d)\n", 
           binary, base_binary, value_binary,
           (value_binary == 13) ? "+" : "-", result_binary);
    
    printf("\n");
}


void test_standard_formats(void) {
    printf("Testing Standard Formats\n");
    
    const char *input_int = "42 -123 999";
    int a, b, c;
    int result = oversscanf(input_int, "%d %d %d", &a, &b, &c);
    printf("Integers: %s -> %d, %d, %d (result: %d)\n", input_int, a, b, c, result);
    
    const char *input_float = "3.14 -2.5 0.001";
    float x, y, z;
    result = oversscanf(input_float, "%f %f %f", &x, &y, &z);
    printf("Floats: %s -> %.2f, %.2f, %.3f (result: %d)\n", input_float, x, y, z, result);
    
    const char *input_string = "Hello World 123";
    char str1[20], str2[20];
    int num;
    result = oversscanf(input_string, "%s %s %d", str1, str2, &num);
    printf("Strings+Int: %s -> '%s', '%s', %d (result: %d)\n", input_string, str1, str2, num, result);
    
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
    
    const char *mixed_with_standard = "MMXXIV 1101 Test 3.14";
    int roman_val2, decimal_val2;
    unsigned int zeck_val2;
    char str[20];
    float float_val;
    
    result = oversscanf(mixed_with_standard, "%Ro %Zr %s %f", 
                       &roman_val2, &zeck_val2, str, &float_val);
    
    printf("Input: \"%s\"\n", mixed_with_standard);
    printf("Parsed: Roman=%d, Zeckendorf=%u, String='%s', Float=%.2f (result: %d)\n",
           roman_val2, zeck_val2, str, float_val, result);
    printf("\n");
}


void test_error_cases(void) {
    printf("Testing Error Cases\n");
    
    const char *invalid_roman = "IIII";
    int roman_val;
    int result = oversscanf(invalid_roman, "%Ro", &roman_val);
    printf("Invalid Roman '%s': result=%d (expected error)\n", invalid_roman, result);
    
    const char *invalid_zeck = "11";
    unsigned int zeck_val;
    result = oversscanf(invalid_zeck, "%Zr", &zeck_val);
    printf("Invalid Zeckendorf '%s': result=%d (expected error)\n", invalid_zeck, result);
    
    const char *invalid_custom = "xyz";
    int custom_val;
    result = oversscanf(invalid_custom, "%Cv", &custom_val, 10);
    printf("Invalid Custom '%s': result=%d (expected error)\n", invalid_custom, result);
    
    printf("\n");
}


int main(void) {
    printf("Overfscanf/OverSscanf Demonstration\n\n");
    
    test_roman_numerals();
    test_zeckendorf();
    test_custom_base();
    test_standard_formats();
    test_mixed_formats();
    test_error_cases();
    
    printf("Testing File Input\n");
    
    FILE *test_file = fopen("test_input.txt", "w");
    if (test_file) {
        fprintf(test_file, "XXV 1001 1f 100\n");
        fclose(test_file);
    } else {
        printf("Error creating test file\n");
        return 1;
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
    } else {
        printf("Error opening test file\n");
        return 1;
    }
    
    return 0;
}