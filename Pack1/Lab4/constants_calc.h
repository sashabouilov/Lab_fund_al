#ifndef CONSTANTS_CALC_H
#define CONSTANTS_CALC_H

#include <stdbool.h>

typedef enum {
    SUCCESS,
    ERROR_INVALID_INPUT,
    ERROR_DIVERGENCE,
    ERROR_MEMORY,
    ERROR_NON_CONVERGENCE
} CalcStatus;

// Вспомогательные функции
bool parse_double(const char *str, double *value);

// Вычисление констант тремя способами

// e
CalcStatus compute_e_limit(double eps, double *result);
CalcStatus compute_e_series(double eps, double *result);
CalcStatus solve_ln_x_eq_1(double eps, double *result);

// pi
CalcStatus compute_pi_limit(double eps, double *result);
CalcStatus compute_pi_series(double eps, double *result);
CalcStatus solve_cos_x_eq_minus_1(double eps, double *result);

// ln 2
CalcStatus compute_ln2_limit(double eps, double *result);
CalcStatus compute_ln2_series(double eps, double *result);
CalcStatus solve_exp_x_eq_2(double eps, double *result);

// sqrt(2)
CalcStatus compute_sqrt2_limit(double eps, double *result);
CalcStatus compute_sqrt2_product(double eps, double *result);
CalcStatus solve_x_squared_eq_2(double eps, double *result);

// gamma (постоянная Эйлера)
CalcStatus compute_gamma_limit(double eps, double *result);
CalcStatus compute_gamma_series(double eps, double *result);
CalcStatus solve_gamma_equation(double eps, double *result);

// Основная функция вывода
void compute_and_print(double epsilon);

#endif