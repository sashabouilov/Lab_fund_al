#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "operations.h"


void print_usage() {
    printf("Usage:\n");
    printf("  -q <epsilon> <a> <b> <c> - Solve quadratic equation with all permutations\n");
    printf("  -m <num1> <num2>          - Check if first number is multiple of second\n");
    printf("  -t <epsilon> <a> <b> <c>  - Check if numbers can form a right triangle\n");
}


bool parse_double(const char *str, double *value) {
    if (str == NULL || value == NULL) return false;
    
    char *endptr;
    errno = 0;
    double result = strtod(str, &endptr);
    
    if (endptr == str || *endptr != '\0' || errno != 0) {
        return false;
    }
    
    *value = result;
    return true;
}


bool parse_int(const char *str, int *value) {
    if (str == NULL || value == NULL) return false;
    
    char *endptr;
    errno = 0;
    long result = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0' || errno != 0 || result < -2147483648 || result > 2147483647) {
        return false;
    }
    
    *value = (int)result;
    return true;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: No flag provided\n");
        print_usage();
        return 1;
    }

    char *flag = argv[1];
    
    if (strcmp(flag, "-q") == 0 || strcmp(flag, "/q") == 0) {
        if (argc != 6) {
            fprintf(stderr, "Error: For -q flag, expected 5 arguments (flag + 4 numbers), got %d\n", argc);
            return 1;
        }
        
        double epsilon, a, b, c;
        if (!parse_double(argv[2], &epsilon) || !parse_double(argv[3], &a) || 
            !parse_double(argv[4], &b) || !parse_double(argv[5], &c)) {
            fprintf(stderr, "Error: Invalid number format\n");
            return 1;
        }
        
        if (epsilon <= 0) {
            fprintf(stderr, "Error: Epsilon must be positive\n");
            return 1;
        }
        
        solve_quadratic_with_permutations(epsilon, a, b, c);
    }
    else if (strcmp(flag, "-m") == 0 || strcmp(flag, "/m") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Error: For -m flag, expected 3 arguments (flag + 2 numbers), got %d\n", argc);
            return 1;
        }
        
        int num1, num2;
        if (!parse_int(argv[2], &num1) || !parse_int(argv[3], &num2)) {
            fprintf(stderr, "Error: Invalid integer format\n");
            return 1;
        }
        
        if (num1 == 0 || num2 == 0) {
            fprintf(stderr, "Error: Both numbers must be non-zero\n");
            return 1;
        }
        
        check_multiple(num1, num2);
    }
    else if (strcmp(flag, "-t") == 0 || strcmp(flag, "/t") == 0) {
        if (argc != 6) {
            fprintf(stderr, "Error: For -t flag, expected 5 arguments (flag + 4 numbers), got %d\n", argc);
            return 1;
        }
        
        double epsilon, a, b, c;
        if (!parse_double(argv[2], &epsilon) || !parse_double(argv[3], &a) || 
            !parse_double(argv[4], &b) || !parse_double(argv[5], &c)) {
            fprintf(stderr, "Error: Invalid number format\n");
            return 1;
        }
        
        if (epsilon <= 0) {
            fprintf(stderr, "Error: Epsilon must be positive\n");
            return 1;
        }
        
        if (a <= 0 || b <= 0 || c <= 0) {
            fprintf(stderr, "Error: All triangle sides must be positive\n");
            return 1;
        }
        
        check_right_triangle(epsilon, a, b, c);
    }
    else {
        fprintf(stderr, "Error: Unknown flag '%s'\n", flag);
        print_usage();
        return 1;
    }

    return 0;
}