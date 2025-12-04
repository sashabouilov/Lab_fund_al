#include "integral.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>


int main(int argc, char *argv[]) {
    double eps = DEFAULT_EPS;
    
    if (argc > 2) {
        fprintf(stderr, "Error: too many arguments\n");
        print_help(argv[0]);
        return EXIT_FAILURE;
    }
    
    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            print_help(argv[0]);
            return EXIT_SUCCESS;
        }
        
        char *endptr;
        errno = 0;
        eps = strtod(argv[1], &endptr);
        
        if (errno == ERANGE || endptr == argv[1] || *endptr != '\0') {
            fprintf(stderr, "Error: invalid epsilon value\n");
            print_help(argv[0]);
            return EXIT_FAILURE;
        }
        
        if (eps <= 0.0 || !isfinite(eps)) {
            fprintf(stderr, "Error: epsilon must be a positive finite number\n");
            print_help(argv[0]);
            return EXIT_FAILURE;
        }
    }
    
    printf("Calculation of integrals by trapezoidal method\n");
    printf("Accuracy e = %g\n\n", eps);
    
    MathFunction functions[] = {
        function_a,
        function_b, 
        function_c,
        function_d
    };
    
    const char *names[] = {
        "Integral a",
        "Integral b",
        "Integral c",
        "Integral d"
    };
    
    const char *descriptions[] = {
        "Integral a",
        "Integral b",
        "Integral c",
        "Integral d"
    };
    
    const size_t num_functions = sizeof(functions) / sizeof(functions[0]);
    IntegralResult results[num_functions];
    
    for (size_t i = 0; i < num_functions; i++) {
        results[i].name = names[i];
        results[i].description = descriptions[i];
        
        if (i == 2) {
            results[i].status = integrate_trapezoidal_singular(
                functions[i], 
                0.0, 1.0, 
                eps, 
                &results[i].result, 
                &results[i].iterations
            );
        } else {
            results[i].status = integrate_trapezoidal(
                functions[i], 
                0.0, 1.0, 
                eps, 
                &results[i].result, 
                &results[i].iterations
            );
        }
    }
    
    for (size_t i = 0; i < num_functions; i++) {
        print_integral_result(&results[i], i);
    }
    
    printf("\n");
    printf("Notes:\n");
    printf("1. Integral 3 has a singularity at x=1 (function approaches infinity)\n");
    printf("2. For integral 3, integration is performed up to x = 1 - %.0e\n", SINGULARITY_EPS);
    printf("3. The true value of integral 3 is 1 (known mathematical result)\n");
    printf("4. Trapezoidal method automatically adapts the number of subdivisions\n");
    printf("5. Maximum number of iterations: %d\n", MAX_ITERATIONS);
    
    return EXIT_SUCCESS;
}