#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdbool.h>

typedef enum {
    QUADRATIC_SUCCESS = 0,
    QUADRATIC_INVALID_PARAMS = 1,
    QUADRATIC_NO_REAL_ROOTS = 2,
    QUADRATIC_LINEAR_EQUATION = 3,
    QUADRATIC_IDENTITY = 4
} QuadraticStatus;

typedef struct {
    double root1;
    double root2;
    int root_count;
} QuadraticSolution;

void solve_quadratic_with_permutations(double epsilon, double a, double b, double c);
QuadraticStatus solve_quadratic_equation(double epsilon, double a, double b, double c, QuadraticSolution *solution);
void print_solution(const QuadraticSolution *solution);

void check_multiple(int num1, int num2);

void check_right_triangle(double epsilon, double a, double b, double c);

#endif