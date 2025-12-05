#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <limits.h>


int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <number> <flag>\n", argv[0]);
        printf("Available flags: -h, -p, -s, -e, -a, -f\n");
        return ERROR_INVALID_INPUT;
    }

    char *endptr;
    errno = 0;
    long x_long = strtol(argv[1], &endptr, 10);
    
    if (errno == ERANGE || x_long > INT_MAX || x_long < INT_MIN) {
        printf("Error: Number out of range\n");
        return ERROR_OUT_OF_RANGE;
    }
    
    if (*endptr != '\0') {
        printf("Error: First argument must be a valid integer\n");
        return ERROR_INVALID_INPUT;
    }
    
    int x = (int)x_long;
    
    if (x < 0) {
        printf("Error: Number must be non-negative\n");
        return ERROR_INVALID_INPUT;
    }

    char *flag = argv[2];
    StatusCode status = SUCCESS;

    if (strcmp(flag, "-h") == 0 || strcmp(flag, "/h") == 0) {
        int *multiples = NULL;
        int count = 0;
        
        status = flag_h(x, &multiples, &count);
        if (status == SUCCESS) {
            print_multiples(multiples, count);
            free(multiples);
        } else {
            printf("No multiples found or invalid input\n");
        }
        
    } else if (strcmp(flag, "-p") == 0 || strcmp(flag, "/p") == 0) {
        bool prime = flag_p(x);
        print_prime_info(x, prime);
        
    } else if (strcmp(flag, "-s") == 0 || strcmp(flag, "/s") == 0) {
        char *digits = NULL;
        
        status = flag_s(x, &digits);
        if (status == SUCCESS) {
            int temp = x;
            int length = 0;
            do {
                length++;
                temp /= 10;
            } while (temp > 0);
            
            print_digits(digits, length);
            free(digits);
        }
        
    } else if (strcmp(flag, "-e") == 0 || strcmp(flag, "/e") == 0) {
        long long **table = NULL;
        
        status = flag_e(x, &table);
        if (status == SUCCESS) {
            print_power_table(table, x);
            for (int i = 0; i < 10; i++) {
                free(table[i]);
            }
            free(table);
        }
        
    } else if (strcmp(flag, "-a") == 0 || strcmp(flag, "/a") == 0) {
        long long sum = 0;
        
        status = flag_a(x, &sum);
        if (status == SUCCESS) {
            print_sum(sum);
        }
        
    } else if (strcmp(flag, "-f") == 0 || strcmp(flag, "/f") == 0) {
        long long factorial = flag_f(x);
        if (factorial > 0) {
            print_factorial(factorial);
        } else {
            printf("Error: Cannot calculate factorial for number %d\n", x);
            status = ERROR_NUMBER_TOO_LARGE;
        }
        
    } else {
        printf("Error: Invalid flag. Available flags: -h, -p, -s, -e, -a, -f\n");
        return ERROR_INVALID_INPUT;
    }

    return status;
}