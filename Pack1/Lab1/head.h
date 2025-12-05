#ifndef HEAD_H
#define HEAD_H

#include <stdbool.h>

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_INPUT = 1,
    ERROR_NO_RESULTS = 2,
    ERROR_OUT_OF_RANGE = 3,
    ERROR_MEMORY_ALLOCATION = 4,
    ERROR_NUMBER_TOO_LARGE = 5
} StatusCode;

int flag_h(int x, int **result, int *count);
bool flag_p(int x);
int flag_s(int x, char **result);
int flag_e(int max_power, long long ***result);
int flag_a(int x, long long *result);
long long flag_f(int x);

void print_multiples(const int *numbers, int count);
void print_prime_info(int x, bool is_prime);
void print_digits(const char *digits, int count);
void print_power_table(long long **table, int max_power);
void print_sum(long long sum);
void print_factorial(long long factorial);

#endif