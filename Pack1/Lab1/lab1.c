#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int flag_h(int x, int **result, int *count) {
    if (x <= 0 || x > 100) {
        return ERROR_NO_RESULTS;
    }
    
    *count = 100 / x;
    if (*count == 0) {
        return ERROR_NO_RESULTS;
    }
    
    *result = (int*)malloc(*count * sizeof(int));
    if (*result == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    for (int i = 1; i <= *count; i++) {
        (*result)[i-1] = x * i;
    }
    
    return SUCCESS;
}


bool flag_p(int x) {
    if (x <= 1) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;
    
    for (int i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
            return false;
        }
    }
    
    return true;
}


int flag_s(int x, char **result) {
    if (x < 0) {
        return ERROR_INVALID_INPUT;
    }
    
    if (x == 0) {
        *result = (char*)malloc(2 * sizeof(char));
        if (*result == NULL) return ERROR_MEMORY_ALLOCATION;
        (*result)[0] = '0';
        (*result)[1] = '\0';
        return SUCCESS;
    }
    
    int temp = x;
    int length = 0;
    while (temp > 0) {
        length++;
        temp /= 10;
    }
    
    *result = (char*)malloc((length + 1) * sizeof(char));
    if (*result == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    temp = x;
    for (int i = length - 1; i >= 0; i--) {
        (*result)[i] = '0' + (temp % 10);
        temp /= 10;
    }
    (*result)[length] = '\0';
    
    return SUCCESS;
}


int flag_e(int max_power, long long ***result) {
    if (max_power < 1 || max_power > 10) {
        return ERROR_OUT_OF_RANGE;
    }
    
    *result = (long long**)malloc(10 * sizeof(long long*));
    if (*result == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    for (int i = 0; i < 10; i++) {
        (*result)[i] = (long long*)malloc(max_power * sizeof(long long));
        if ((*result)[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free((*result)[j]);
            }
            free(*result);
            return ERROR_MEMORY_ALLOCATION;
        }
        
        long long power = 1;
        for (int j = 0; j < max_power; j++) {
            power *= (i + 1);
            (*result)[i][j] = power;
        }
    }
    
    return SUCCESS;
}


int flag_a(int x, long long *result) {
    if (x < 0) {
        return ERROR_INVALID_INPUT;
    }
    
    *result = ((long long)x * (x + 1)) / 2;
    return SUCCESS;
}


long long flag_f(int x) {
    if (x < 0) return -1;
    if (x == 0) return 1;
    
    long long result = 1;
    for (int i = 1; i <= x; i++) {
        if (result > LLONG_MAX / i) {
            return -1;
        }
        result *= i;
    }
    return result;
}


void print_multiples(const int *numbers, int count) {
    if (count == 0) {
        printf("No multiples found\n");
        return;
    }
    
    printf("Multiples of the number up to 100: ");
    for (int i = 0; i < count; i++) {
        printf("%d", numbers[i]);
        if (i < count - 1) printf(", ");
    }
    printf("\n");
}


void print_prime_info(int x, bool is_prime_flag) {
    if (x <= 1) {
        printf("%d is neither prime nor composite\n", x);
    } else if (is_prime_flag) {
        printf("%d is a prime number\n", x);
    } else {
        printf("%d is a composite number\n", x);
    }
}


void print_digits(const char *digits, int count) {
    printf("Digits of the number: ");
    for (int i = 0; i < count; i++) {
        printf("%c", digits[i]);
        if (i < count - 1) printf(" ");
    }
    printf("\n");
}

void print_power_table(long long **table, int max_power) {
    printf("Power table (numbers 1-10):\n");
    printf("Base | Powers\n");
    
    for (int i = 0; i < 10; i++) {
        printf("%4d | ", i + 1);
        for (int j = 0; j < max_power; j++) {
            printf("%lld ", table[i][j]);
        }
        printf("\n");
    }
}

void print_sum(long long sum) {
    printf("Sum of numbers from 1 to N: %lld\n", sum);
}

void print_factorial(long long factorial) {
    printf("Factorial: %lld\n", factorial);
}