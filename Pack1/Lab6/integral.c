#include "integral.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>


int validate_input_parameters(double a, double b, double eps) {
    if (isnan(a) || isnan(b) || isnan(eps)) {
        return 0;
    }
    if (!isfinite(a) || !isfinite(b) || !isfinite(eps)) {
        return 0;
    }
    if (eps <= 0.0) {
        return 0;
    }
    if (b <= a) {
        return 0;
    }
    return 1;
}


IntegralStatus integrate_trapezoidal(MathFunction f, double a, double b, 
                                     double eps, double *result, int *iterations) {
    if (f == NULL || result == NULL || iterations == NULL) {
        return INTEGRAL_ERROR_NULL_POINTER;
    }
    
    if (!validate_input_parameters(a, b, eps)) {
        return INTEGRAL_ERROR_INVALID_INPUT;
    }
    
    *iterations = 0;
    int n = 1;
    double h = b - a;
    
    IntegralStatus func_status_a, func_status_b;
    double fa = f(a, &func_status_a);
    double fb = f(b, &func_status_b);
    
    if (func_status_a != INTEGRAL_SUCCESS || func_status_b != INTEGRAL_SUCCESS) {
        return INTEGRAL_ERROR_MATH_DOMAIN;
    }
    
    double T_old = 0.5 * h * (fa + fb);
    double T_new = T_old;
    
    for (int iter = 1; iter <= MAX_ITERATIONS; iter++) {
        *iterations = iter;
        
        double sum = 0.0;
        int valid_points = 0;
        
        for (int i = 1; i <= n; i++) {
            double x = a + (i - 0.5) * h;
            IntegralStatus func_status;
            double fx = f(x, &func_status);
            
            if (func_status != INTEGRAL_SUCCESS) {
                continue;
            }
            
            sum += fx;
            valid_points++;
        }
        
        if (valid_points == 0) {
            *result = T_new;
            return INTEGRAL_ERROR_MATH_DOMAIN;
        }
        
        T_new = 0.5 * (T_old + h * sum);
        
        if (iter > 1 && fabs(T_new - T_old) < eps) {
            *result = T_new;
            return INTEGRAL_SUCCESS;
        }
        
        T_old = T_new;
        n *= 2;
        h /= 2.0;
    }
    
    *result = T_new;
    return INTEGRAL_ERROR_TOO_MANY_ITERATIONS;
}


IntegralStatus integrate_trapezoidal_singular(MathFunction f, double a, double b,
                                              double eps, double *result, int *iterations) {
    if (f == NULL || result == NULL || iterations == NULL) {
        return INTEGRAL_ERROR_NULL_POINTER;
    }
    
    if (!validate_input_parameters(a, b - SINGULARITY_EPS, eps)) {
        return INTEGRAL_ERROR_INVALID_INPUT;
    }
    
    return integrate_trapezoidal(f, a, b - SINGULARITY_EPS, eps, result, iterations);
}


double function_a(double x, IntegralStatus *status) {
    if (status != NULL) {
        *status = INTEGRAL_SUCCESS;
    }
    
    if (x < 0.0 || x > 1.0) {
        if (status != NULL) {
            *status = INTEGRAL_ERROR_MATH_DOMAIN;
        }
        return NAN;
    }
    
    if (x == 0.0) {
        return 1.0;
    }
    
    double value = log(1.0 + x) / x;
    
    if (!isfinite(value)) {
        if (status != NULL) {
            *status = INTEGRAL_ERROR_MATH_DOMAIN;
        }
        return NAN;
    }
    
    return value;
}


double function_b(double x, IntegralStatus *status) {
    if (status != NULL) {
        *status = INTEGRAL_SUCCESS;
    }
    
    if (x < 0.0 || x > 1.0) {
        if (status != NULL) {
            *status = INTEGRAL_ERROR_MATH_DOMAIN;
        }
        return NAN;
    }
    
    double value = exp(-x * x / 2.0);
    
    if (!isfinite(value)) {
        if (status != NULL) {
            *status = INTEGRAL_ERROR_MATH_DOMAIN;
        }
        return NAN;
    }
    
    return value;
}


double function_c(double x, IntegralStatus *status) {
    if (status != NULL) {
        *status = INTEGRAL_SUCCESS;
    }
    
    if (x < 0.0 || x > 1.0) {
        if (status != NULL) {
            *status = INTEGRAL_ERROR_MATH_DOMAIN;
        }
        return NAN;
    }
    
    if (x == 1.0) {
        if (status != NULL) {
            *status = INTEGRAL_ERROR_SINGULARITY;
        }
        return INFINITY;
    }
    
    double value = -log(1.0 - x);
    
    if (!isfinite(value)) {
        if (status != NULL) {
            *status = INTEGRAL_ERROR_MATH_DOMAIN;
        }
        return NAN;
    }
    
    return value;
}


double function_d(double x, IntegralStatus *status) {
    if (status != NULL) {
        *status = INTEGRAL_SUCCESS;
    }
    
    if (x < 0.0 || x > 1.0) {
        if (status != NULL) {
            *status = INTEGRAL_ERROR_MATH_DOMAIN;
        }
        return NAN;
    }
    
    if (x == 0.0) {
        return 1.0;
    }
    
    double log_x = log(x);
    if (!isfinite(log_x)) {
        if (status != NULL) {
            *status = INTEGRAL_ERROR_MATH_DOMAIN;
        }
        return NAN;
    }
    
    double exponent = x * log_x;
    double value = exp(exponent);
    
    if (!isfinite(value)) {
        if (status != NULL) {
            *status = INTEGRAL_ERROR_MATH_DOMAIN;
        }
        return NAN;
    }
    
    return value;
}


void print_integral_result(const IntegralResult *result, size_t index) {
    if (result == NULL) {
        return;
    }
    
    printf("Integral %zu: %s\n", index + 1, result->name);
    if (result->description != NULL) {
        printf("Description: %s\n", result->description);
    }
    
    printf("Result: ");
    
    switch (result->status) {
        case INTEGRAL_SUCCESS:
            printf("%.10g", result->result);
            break;
        case INTEGRAL_ERROR_TOO_MANY_ITERATIONS:
            printf("%.10g (iteration limit reached)", result->result);
            break;
        case INTEGRAL_ERROR_MATH_DOMAIN:
            printf("NAN (function undefined on part of interval)");
            break;
        case INTEGRAL_ERROR_SINGULARITY:
            printf("NAN (singularity at boundary)");
            break;
        case INTEGRAL_ERROR_INVALID_INPUT:
            printf("Invalid input error");
            break;
        case INTEGRAL_ERROR_NULL_POINTER:
            printf("Error: null pointer");
            break;
        default:
            printf("Unknown error");
    }
    
    printf("\nIterations: %d\n", result->iterations);
    
    const char *status_msg;
    switch (result->status) {
        case INTEGRAL_SUCCESS:
            status_msg = "Success";
            break;
        case INTEGRAL_ERROR_TOO_MANY_ITERATIONS:
            status_msg = "Too many iterations";
            break;
        case INTEGRAL_ERROR_MATH_DOMAIN:
            status_msg = "Math domain error";
            break;
        case INTEGRAL_ERROR_SINGULARITY:
            status_msg = "Singularity at boundary";
            break;
        case INTEGRAL_ERROR_INVALID_INPUT:
            status_msg = "Invalid input";
            break;
        default:
            status_msg = "Error";
    }
    
    printf("Status: %s\n\n", status_msg);
}


void print_help(const char *program_name) {
    printf("Calculation of integrals by trapezoidal method\n");
    printf("Usage: %s [epsilon]\n\n", program_name);
    printf("Arguments:\n");
    printf("epsilon - calculation accuracy (positive real number)\n");
    printf("if not specified, default value is used: %g\n\n", DEFAULT_EPS);
    printf("Calculated integrals:\n");
    printf("1. Integral a\n");
    printf("2. Integral b\n");
    printf("3. Integral c\n");
    printf("4. Integral d\n\n");
    printf("Examples:\n");
    printf("%s 0.0001\n", program_name);
    printf("%s 1e-6\n", program_name);
    printf("%s\n", program_name);
}