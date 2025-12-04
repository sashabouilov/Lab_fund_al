#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <stddef.h>

#define MAX_ITERATIONS 1000000
#define DEFAULT_EPS 1e-6
#define SINGULARITY_EPS 1e-12

typedef enum {
    INTEGRAL_SUCCESS = 0,
    INTEGRAL_ERROR_INVALID_INPUT,
    INTEGRAL_ERROR_ALLOCATION_FAILED,
    INTEGRAL_ERROR_TOO_MANY_ITERATIONS,
    INTEGRAL_ERROR_NULL_POINTER,
    INTEGRAL_ERROR_MATH_DOMAIN,
    INTEGRAL_ERROR_SINGULARITY
} IntegralStatus;

typedef struct {
    double result;
    int iterations;
    IntegralStatus status;
    const char *name;
    const char *description;
} IntegralResult;

typedef double (*MathFunction)(double x, IntegralStatus *status);

IntegralStatus integrate_trapezoidal(MathFunction f, double a, double b, 
                                     double eps, double *result, int *iterations);
IntegralStatus integrate_trapezoidal_singular(MathFunction f, double a, double b,
                                              double eps, double *result, int *iterations);

double function_a(double x, IntegralStatus *status);
double function_b(double x, IntegralStatus *status);
double function_c(double x, IntegralStatus *status);
double function_d(double x, IntegralStatus *status);

int validate_input_parameters(double a, double b, double eps);
void print_integral_result(const IntegralResult *result, size_t index);
void print_help(const char *program_name);

#endif