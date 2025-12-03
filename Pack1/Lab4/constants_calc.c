#include "constants_calc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define M_PI 3.14159265358979323846


bool parse_double(const char *str, double *value) {
    char *endptr;
    *value = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') {
        return false;
    }
    return true;
}


CalcStatus compute_e_limit(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    long long n = 1;
    double prev, curr = pow(1.0 + 1.0 / n, n);
    do {
        prev = curr;
        n *= 2;
        curr = pow(1.0 + 1.0 / n, n);
        if (n > 1e12) return ERROR_DIVERGENCE;
    } while (fabs(curr - prev) > eps);
    *result = curr;
    return SUCCESS;
}


CalcStatus compute_e_series(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double sum = 1.0, term = 1.0;
    int n = 1;
    while (fabs(term) > eps) {
        term /= n;
        sum += term;
        n++;
        if (n > 1000000) return ERROR_DIVERGENCE;
    }
    *result = sum;
    return SUCCESS;
}


CalcStatus solve_ln_x_eq_1(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double x_low = 0.1, x_high = 10.0;
    for (int i = 0; i < 1000; i++) {
        double x_mid = (x_low + x_high) / 2.0;
        double val = log(x_mid) - 1.0;
        if (fabs(val) < eps) {
            *result = x_mid;
            return SUCCESS;
        }
        if (val > 0) x_high = x_mid;
        else x_low = x_mid;
    }
    return ERROR_NON_CONVERGENCE;
}


CalcStatus compute_pi_limit(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double product = 1.0;
    double prev_product = 0.0;
    int n = 1;
    do {
        prev_product = product;
        double term = (4.0 * n * n) / (4.0 * n * n - 1.0);
        product *= term;
        n++;
        if (n > 5000000) return ERROR_DIVERGENCE;
    } while (fabs(2.0 * product - 2.0 * prev_product) > eps);
    *result = 2.0 * product;
    return SUCCESS;
}


CalcStatus compute_pi_series(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double sum = 0.0;
    double prev_sum;
    int n = 1;
    do {
        prev_sum = sum;
        double term = 4.0 * ((n % 2 == 1) ? 1.0 : -1.0) / (2.0 * n - 1.0);
        sum += term;
        n++;
        if (n > 10000000) return ERROR_DIVERGENCE;
    } while (fabs(sum - prev_sum) > eps);
    *result = sum;
    return SUCCESS;
}


CalcStatus solve_cos_x_eq_minus_1(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    
    double x_low = 3.0, x_high = 3.5;
    double x = (x_low + x_high) / 2.0;
    double best_x = x;
    double best_error = fabs(cos(x) + 1.0);
    
    for (int i = 0; i < 20; i++) {
        double x_mid = (x_low + x_high) / 2.0;
        double val = cos(x_mid) + 1.0;
        
        double error = fabs(val);
        if (error < best_error) {
            best_error = error;
            best_x = x_mid;
        }
        
        if (error < eps) {
            *result = x_mid;
            return SUCCESS;
        }
        
        if (val > 0) x_high = x_mid;
        else x_low = x_mid;
    }
    
    x = best_x;
    for (int i = 0; i < 20; i++) {
        double fx = cos(x) + 1.0;
        double dfx = -sin(x);
        
        if (fabs(dfx) < 1e-15) {
            break;
        }
        
        double x_new = x - fx / dfx;
        
        if (x_new < 3.0 || x_new > 3.5) {
            break;
        }
        
        double new_error = fabs(cos(x_new) + 1.0);
        if (new_error < best_error) {
            best_error = new_error;
            best_x = x_new;
        }
        
        if (new_error < eps) {
            *result = x_new;
            return SUCCESS;
        }
        
        x = x_new;
    }
    
    *result = best_x;
    return (best_error < eps * 100) ? SUCCESS : ERROR_NON_CONVERGENCE;
}


CalcStatus compute_ln2_limit(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double prev = 0.0, curr = 0.0;
    int n = 1;
    do {
        prev = curr;
        curr = n * (pow(2.0, 1.0 / n) - 1.0);
        n++;
        if (n > 1000000) return ERROR_DIVERGENCE;
    } while (fabs(curr - prev) > eps);
    *result = curr;
    return SUCCESS;
}


CalcStatus compute_ln2_series(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double sum = 0.0;
    double prev_sum;
    int n = 1;
    do {
        prev_sum = sum;
        double term = ((n % 2 == 1) ? 1.0 : -1.0) / n;
        sum += term;
        n++;
        if (n > 10000000) return ERROR_DIVERGENCE;
    } while (fabs(sum - prev_sum) > eps);
    *result = sum;
    return SUCCESS;
}


CalcStatus solve_exp_x_eq_2(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double x_low = 0.0, x_high = 1.0;
    double best_x = 0.5;
    double best_error = fabs(exp(best_x) - 2.0);
    
    for (int i = 0; i < 100; i++) {
        double x_mid = (x_low + x_high) / 2.0;
        double val = exp(x_mid) - 2.0;
        
        double error = fabs(val);
        if (error < best_error) {
            best_error = error;
            best_x = x_mid;
        }
        
        if (error < eps) {
            *result = x_mid;
            return SUCCESS;
        }
        
        if (val > 0) x_high = x_mid;
        else x_low = x_mid;
    }
    
    *result = best_x;
    return (best_error < eps * 10) ? SUCCESS : ERROR_NON_CONVERGENCE;
}


CalcStatus compute_sqrt2_limit(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double x = -0.5;
    double prev;
    int iter = 0;
    do {
        prev = x;
        x = x - x * x / 2.0 + 1.0;
        iter++;
        if (iter > 10000) return ERROR_NON_CONVERGENCE;
    } while (fabs(x - prev) > eps);
    *result = x;
    return SUCCESS;
}


CalcStatus compute_sqrt2_product(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double prod = 1.0;
    double prev;
    int k = 2;
    do {
        prev = prod;
        prod *= pow(2.0, pow(2.0, -k));
        k++;
        if (k > 10000) return ERROR_DIVERGENCE;
    } while (fabs(prod - prev) > eps);
    *result = prod;
    return SUCCESS;
}


CalcStatus solve_x_squared_eq_2(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double x_low = 1.0, x_high = 2.0;
    double best_x = 1.5;
    double best_error = fabs(best_x * best_x - 2.0);
    
    for (int i = 0; i < 100; i++) {
        double x_mid = (x_low + x_high) / 2.0;
        double val = x_mid * x_mid - 2.0;
        
        double error = fabs(val);
        if (error < best_error) {
            best_error = error;
            best_x = x_mid;
        }
        
        if (error < eps) {
            *result = x_mid;
            return SUCCESS;
        }
        
        if (val > 0) x_high = x_mid;
        else x_low = x_mid;
    }
    
    *result = best_x;
    return (best_error < eps * 10) ? SUCCESS : ERROR_NON_CONVERGENCE;
}


CalcStatus compute_gamma_limit(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double H_n = 1.0;
    double prev = 0.0, curr = 1.0 - log(1.0);
    int n = 1;
    do {
        prev = curr;
        n++;
        H_n += 1.0 / n;
        curr = H_n - log(n);
        if (n > 10000000) return ERROR_DIVERGENCE;
    } while (fabs(curr - prev) > eps);
    *result = curr;
    return SUCCESS;
}


CalcStatus compute_gamma_series(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    double sum = 0.0;
    double prev_sum;
    int n = 1;
    do {
        prev_sum = sum;
        sum += (1.0 / n - log(1.0 + 1.0 / n));
        n++;
        if (n > 10000000) return ERROR_DIVERGENCE;
    } while (fabs(sum - prev_sum) > eps);
    *result = sum;
    return SUCCESS;
}


CalcStatus solve_gamma_equation(double eps, double *result) {
    if (eps <= 0) return ERROR_INVALID_INPUT;
    const int t = 100000;
    double product = 1.0;
    
    char *is_prime = calloc(t + 1, sizeof(char));
    if (!is_prime) return ERROR_MEMORY;
    
    for (int i = 2; i <= t; i++) is_prime[i] = 1;
    for (int p = 2; p * p <= t; p++) {
        if (is_prime[p]) {
            for (int i = p * p; i <= t; i += p) {
                is_prime[i] = 0;
            }
        }
    }
    
    for (int p = 2; p <= t; p++) {
        if (is_prime[p]) {
            product *= (p - 1.0) / p;
        }
    }
    
    free(is_prime);
    
    double rhs = log(t) * product;
    double gamma_approx = -log(rhs);
    
    *result = gamma_approx;
    return SUCCESS;
}


void compute_and_print(double epsilon) {
    const char *status_str[] = {
        "SUCCESS",
        "ERROR_INVALID_INPUT",
        "ERROR_DIVERGENCE",
        "ERROR_MEMORY",
        "ERROR_NON_CONVERGENCE"
    };

    struct {
        const char *name;
        CalcStatus (*limit)(double, double*);
        CalcStatus (*series)(double, double*);
        CalcStatus (*equation)(double, double*);
    } constants[] = {
        {"e", compute_e_limit, compute_e_series, solve_ln_x_eq_1},
        {"pi", compute_pi_limit, compute_pi_series, solve_cos_x_eq_minus_1},
        {"ln(2)", compute_ln2_limit, compute_ln2_series, solve_exp_x_eq_2},
        {"sqrt(2)", compute_sqrt2_limit, compute_sqrt2_product, solve_x_squared_eq_2},
        {"gamma", compute_gamma_limit, compute_gamma_series, solve_gamma_equation}
    };

    for (size_t i = 0; i < sizeof(constants) / sizeof(constants[0]); i++) {
        printf("\n %s \n", constants[i].name);
        double res;
        CalcStatus s;

        s = constants[i].limit(epsilon, &res);
        printf("Limit:      %.15f [%s]\n", res, status_str[s]);

        s = constants[i].series(epsilon, &res);
        printf("Series/Prod:%.15f [%s]\n", res, status_str[s]);

        s = constants[i].equation(epsilon, &res);
        printf("Equation:   %.15f [%s]\n", res, status_str[s]);
    }
}