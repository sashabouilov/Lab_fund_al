#include <stdio.h>
#include <stdlib.h>
#include "overprintf.h"

int main() {
    printf("Basic overprintf test\n\n");
    
    printf("1. Testing standard specifiers:\n");
    overfprintf(stdout, "   Integer: %d\n", 42);
    overfprintf(stdout, "   String: %s\n", "hello");
    overfprintf(stdout, "   Character: %c\n", 'A');
    
    printf("\n2. Testing Roman numerals:\n");
    overfprintf(stdout, "   2024 in Roman: %Ro\n", 2024);
    overfprintf(stdout, "   123 in Roman: %Ro\n", 123);
    
    printf("\n3. Testing Zeckendorf:\n");
    overfprintf(stdout, "   10 in Zeckendorf: %Zr\n", 10);
    
    printf("\n4. Testing custom base:\n");
    overfprintf(stdout, "   255 in hex: %Cv\n", 255, 16);
    
    printf("\n5. Testing oversprintf:\n");
    char result[256];
    oversprintf(result, "Result: %d + %d = %d", 2, 3, 5);
    printf("   %s\n", result);
    
    printf("\nTest completed\n");
    return 0;
}