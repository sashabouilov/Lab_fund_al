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

typedef enum {
    MULTIPLE_SUCCESS = 0,
    MULTIPLE_DIVISION_BY_ZERO = 1,
    MULTIPLE_INVALID_PARAMS = 2
} MultipleStatus;

typedef enum {
    TRIANGLE_SUCCESS = 0,
    TRIANGLE_NOT_TRIANGLE = 1,
    TRIANGLE_INVALID_SIDES = 2,
    TRIANGLE_INVALID_PARAMS = 3
} TriangleStatus;

typedef struct {
    double root1;
    double root2;
    int root_count;
} QuadraticSolution;

typedef struct {
    QuadraticSolution solutions[6];
    double coefficients[6][3];
    QuadraticStatus statuses[6];
} PermutationsResult;

QuadraticStatus solve_quadratic_equation(double epsilon, double a, double b, double c, QuadraticSolution *solution);
MultipleStatus check_multiple(int num1, int num2, bool *is_multiple);
TriangleStatus check_right_triangle(double epsilon, double a, double b, double c, bool *is_right_triangle);

QuadraticStatus solve_quadratic_with_permutations(double epsilon, double a, double b, double c, PermutationsResult *result);

void print_solution(const QuadraticSolution *solution);
void print_quadratic_status(QuadraticStatus status);
void print_multiple_result(int num1, int num2, bool is_multiple, MultipleStatus status);
void print_triangle_result(double a, double b, double c, bool is_right_triangle, TriangleStatus status);
void print_permutations_result(const PermutationsResult *result);

#endif