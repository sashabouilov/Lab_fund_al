#include "finite_representation.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>


static int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


static void double_to_fraction(double x, double epsilon, int *numerator, int *denominator) {
    if (fabs(x) < epsilon) {
        *numerator = 0;
        *denominator = 1;
        return;
    }
    
    double integral = floor(x);
    double fractional = x - integral;
    
    if (fractional < epsilon) {
        *numerator = (int)integral;
        *denominator = 1;
        return;
    }
    
    if (fabs(1.0 - fractional) < epsilon) {
        *numerator = (int)integral + 1;
        *denominator = 1;
        return;
    }
    
    int lower_n = 0, lower_d = 1;
    int upper_n = 1, upper_d = 1;
    
    while (1) {
        int middle_n = lower_n + upper_n;
        int middle_d = lower_d + upper_d;
        
        if (middle_d > 1000000) break;

        double middle_val = (double)middle_n / (double)middle_d;
        
        if (fabs(middle_val - x) < epsilon) {
            *numerator = middle_n;
            *denominator = middle_d;
            return;
        }
        
        if (middle_val < x) {
            lower_n = middle_n;
            lower_d = middle_d;
        } else {
            upper_n = middle_n;
            upper_d = middle_d;
        }
    }

    *denominator = 1;
    while (fabs(x * *denominator - round(x * *denominator)) > epsilon && *denominator < 1000000) {
        (*denominator)++;
    }
    *numerator = (int)round(x * *denominator);

    int common_divisor = gcd(*numerator, *denominator);
    *numerator /= common_divisor;
    *denominator /= common_divisor;
}


static bool has_only_base_factors(int denominator, int base) {
    if (denominator == 1) return true; 
    
    int temp = denominator;
    
    for (int i = 2; i <= temp; i++) {
        while (temp % i == 0) {
            if (base % i != 0) return false;

            temp /= i;
        }
    }
    
    return true;
}


bool is_finite_representation(double number, int base, double epsilon) {
    if (base < 2) return false;

    int numerator, denominator;
    double_to_fraction(number, epsilon, &numerator, &denominator);

    return has_only_base_factors(denominator, base);
}

int check_finite_representation(int base, double epsilon, int count, ...) {
    if (base < 2) return 1;

    
    if (count <= 0) return 2;
    
    if (epsilon <= 0.0) epsilon = 1e-12;

    
    va_list args;
    va_start(args, count);
    
    for (int i = 0; i < count; i++) {
        double number = va_arg(args, double);

        if (number <= 0.0 || number >= 1.0) {
            va_end(args);
            return 3;
        }

        bool is_finite = is_finite_representation(number, base, epsilon);

        printf("Fraction %.10f in the base number system %d %sthe final representation\n",
               number, base, is_finite ? "has " : "hasn`t ");
    }
    
    va_end(args);
    return 0;
}