#include <stdio.h>
#include "finite_representation.h"

void run_tests() {
    printf("Testing the representation finiteness check function\n\n");
    
    printf("Test 1: Binary number system (base = 2)\n");
    int result = check_finite_representation(2, 1e-12, 3, 0.5, 0.25, 0.125);
    printf("Status: %d\n\n", result);
    
    printf("Test 2: Decimal number system (base = 10)\n");
    result = check_finite_representation(10, 1e-12, 4, 0.1, 0.2, 0.5, 0.75);
    printf("Status: %d\n\n", result);
    
    printf("Test 3: Octal number system (base = 8)\n");
    result = check_finite_representation(8, 1e-12, 3, 0.125, 0.25, 0.5);
    printf("Status: %d\n\n", result);
    
    printf("Test 4: The Ternary number system (base = 3)\n");
    result = check_finite_representation(3, 1e-12, 2, 1.0/3.0, 2.0/9.0);
    printf("Status: %d\n\n", result);
    
    printf("Test 5: Error Handling Check\n");
    
    result = check_finite_representation(1, 1e-12, 1, 0.5);
    printf("Incorrect basis - status: %d\n", result);
    
    result = check_finite_representation(2, 1e-12, 1, 1.5);
    printf("Fraction out of range - status: %d\n", result);
}

int main() {
    run_tests();
    
    printf("\nDemonstration of working with user input\n");
    
    int base;
    double epsilon = 1e-12;
    int count;
    
    printf("Enter the base of the number system: ");
    if (scanf("%d", &base) != 1 || base < 2) {
        printf("Error: incorrect base of the number system\n");
        return 1;
    }
    
    printf("Enter the number of fractions to check: ");
    if (scanf("%d", &count) != 1 || count <= 0) {
        printf("Error: incorrect number of fractions\n");
        return 1;
    }
    
    printf("Enter %d fractions in the range (0;1):\n", count);
    double numbers[count];
    
    for (int i = 0; i < count; i++) {
        printf("Fraction %d: ", i + 1);
        if (scanf("%lf", &numbers[i]) != 1 || numbers[i] <= 0.0 || numbers[i] >= 1.0) {
            printf("Error: incorrect fraction\n");
            return 1;
        }
    }
    
    switch (count) {
        case 1:
            check_finite_representation(base, epsilon, count, numbers[0]);
            break;
        case 2:
            check_finite_representation(base, epsilon, count, numbers[0], numbers[1]);
            break;
        case 3:
            check_finite_representation(base, epsilon, count, numbers[0], numbers[1], numbers[2]);
            break;
        case 4:
            check_finite_representation(base, epsilon, count, numbers[0], numbers[1], numbers[2], numbers[3]);
            break;
        default:
            printf("Up to 4 fractions are supported for demonstration purposes.\n");
            break;
    }
    
    return 0;
}