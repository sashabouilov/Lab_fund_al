#include <stdio.h>
#include <stdlib.h>
#include "overprintf.h"

int main() {
    printf("Demonstration of overprintf functions\n\n");
    
    printf("1. Roman numerals (%%Ro):\n");
    overfprintf(stdout, "   2024 = %Ro\n", 2024);
    overfprintf(stdout, "   3999 = %Ro\n", 3999);
    overfprintf(stdout, "   123 = %Ro\n", 123);
    overfprintf(stdout, "   0 = %Ro\n", 0);
    
    printf("\n2. Zeckendorf representation (%%Zr):\n");
    overfprintf(stdout, "   123 = %Zr\n", 123);
    overfprintf(stdout, "   100 = %Zr\n", 100);
    overfprintf(stdout, "   1 = %Zr\n", 1);
    overfprintf(stdout, "   0 = %Zr\n", 0);
    
    printf("\n3. Custom base conversion:\n");
    overfprintf(stdout, "   255 in hex (lowercase): %Cv\n", 255, 16);
    overfprintf(stdout, "   255 in hex (uppercase): %CV\n", 255, 16);
    overfprintf(stdout, "   42 in binary: %Cv\n", 42, 2);
    overfprintf(stdout, "   -42 in binary: %Cv\n", -42, 2);
    overfprintf(stdout, "   123 in base 8: %Cv\n", 123, 8);
    
    printf("\n4. String to integer conversion:\n");
    overfprintf(stdout, "   \"FF\" from hex: %to\n", "FF", 16);
    overfprintf(stdout, "   \"1010\" from binary: %to\n", "1010", 2);
    overfprintf(stdout, "   \"1A\" from hex (uppercase): %TO\n", "1A", 16);
    overfprintf(stdout, "   \"77\" from octal: %to\n", "77", 8);
    overfprintf(stdout, "   \"invalid\" from hex: %to\n", "invalid", 16);
    
    printf("\n5. Memory dumps:\n");
    int test_int = 123456789;
    overfprintf(stdout, "   %%mi (int 123456789): %mi\n", test_int);
    
    unsigned int test_uint = 123456789;
    overfprintf(stdout, "   %%mu (unsigned int 123456789): %mu\n", test_uint);
    
    double test_double = 3.14159;
    overfprintf(stdout, "   %%md (double 3.14159): %md\n", test_double);
    
    float test_float = 2.71828f;
    overfprintf(stdout, "   %%mf (float 2.71828): %mf\n", test_float);
    
    printf("\n6. Special cases handling:\n");
    overfprintf(stdout, "   Regular text with %% symbol: 50%% discount\n");
    overfprintf(stdout, "   Unknown specifier %%Xy: %Xy\n");
    overfprintf(stdout, "   Double %% symbol: %%%%\n");
    overfprintf(stdout, "   Mixed: %Ro and then %%%%\n", 42);
    
    printf("\n7. oversprintf demonstration:\n");
    char result[512];
    
    oversprintf(result, "Combined example: number %d in Roman: %Ro", 2024, 2024);
    printf("   %s\n", result);
    
    oversprintf(result, "Number %d in binary: %Cv", 42, 42, 2);
    printf("   %s\n", result);
    
    oversprintf(result, "Text with %% symbol: %d%% complete", 75);
    printf("   %s\n", result);
    
    oversprintf(result, "Multiple conversions: Roman(%Ro) Zeckendorf(%Zr) Binary(%Cv)", 123, 123, 123, 2);
    printf("   %s\n", result);
    
    printf("\n8. Edge cases and error handling:\n");
    overfprintf(stdout, "   Invalid base (50): %Cv\n", 255, 50);
    overfprintf(stdout, "   Negative Roman: %Ro\n", -5);
    overfprintf(stdout, "   Large Zeckendorf: %Zr\n", 1000000);
    
    return 0;
}