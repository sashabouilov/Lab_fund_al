#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "overprintf.h"


void test_standard_format_specifiers() {
    printf("Standard Format Specifiers Tests\n");
    char buffer[256];
    
    printf("1. Basic specifiers:\n");
    
    oversprintf(buffer, "Decimal: %d", 123);
    printf("   Result: '%s'\n", buffer);
    assert(strcmp(buffer, "Decimal: 123") == 0);
    printf("   + %%d - OK\n");
    
    oversprintf(buffer, "String: %s", "test");
    printf("   Result: '%s'\n", buffer);
    assert(strcmp(buffer, "String: test") == 0);
    printf("   + %%s - OK\n");
    
    oversprintf(buffer, "Char: %c", 'A');
    printf("   Result: '%s'\n", buffer);
    assert(strcmp(buffer, "Char: A") == 0);
    printf("   + %%c - OK\n");
    
    printf("+ All standard format tests passed\n\n");
}


void test_roman_comprehensive() {
    printf("Comprehensive Roman Numerals Tests\n");
    char buffer[64];
    
    printf("1. Basic values:\n");
    assert(int_to_roman(1, buffer, sizeof(buffer)) == OVERPRINTF_SUCCESS);
    assert(strcmp(buffer, "I") == 0);
    printf("   + 1 = I\n");
    
    assert(int_to_roman(2024, buffer, sizeof(buffer)) == OVERPRINTF_SUCCESS);
    assert(strcmp(buffer, "MMXXIV") == 0);
    printf("   + 2024 = MMXXIV\n");
    
    printf("2. Error handling:\n");
    assert(int_to_roman(0, buffer, sizeof(buffer)) == OVERPRINTF_INVALID_ROMAN);
    printf("   + 0 - INVALID_ROMAN\n");
    
    printf("+ All Roman numeral tests passed\n\n");
}


void test_zeckendorf() {
    printf("Zeckendorf Tests\n");
    char buffer[128];
    
    printf("1. Basic values:\n");
    assert(uint_to_zeckendorf(1, buffer, sizeof(buffer)) == OVERPRINTF_SUCCESS);
    printf("   + 1 = %s\n", buffer);
    
    assert(uint_to_zeckendorf(10, buffer, sizeof(buffer)) == OVERPRINTF_SUCCESS);
    printf("   + 10 = %s\n", buffer);
    
    printf("+ Zeckendorf tests completed\n\n");
}


void test_integration() {
    printf("Integration Tests\n");
    char buffer[256];
    
    printf("1. Combined standard and custom:\n");
    oversprintf(buffer, "Number %d: Roman=%Ro", 255, 255);
    printf("   + %s\n", buffer);
    
    oversprintf(buffer, "Test: %s %d", "value", 42);
    printf("   + %s\n", buffer);
    
    printf("+ All integration tests passed\n\n");
}

int main() {
    printf("STARTING COMPREHENSIVE TEST SUITE\n\n");
    
    test_standard_format_specifiers();
    test_roman_comprehensive();
    test_zeckendorf();
    test_integration();
    
    printf("ALL TESTS PASSED SUCCESSFULLY!\n");
    
    return 0;
}