#include "operations.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>


static void swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}


QuadraticStatus solve_quadratic_equation(double epsilon, double a, double b, double c, QuadraticSolution *solution) {
    if (solution == NULL) {
        return QUADRATIC_INVALID_PARAMS;
    }
    
    solution->root_count = 0;
    solution->root1 = 0.0;
    solution->root2 = 0.0;
    
    if (fabs(a) <= epsilon) {
        if (fabs(b) <= epsilon) {
            if (fabs(c) <= epsilon) {
                return QUADRATIC_IDENTITY;
            } else {
                return QUADRATIC_NO_REAL_ROOTS;
            }
        } else {
            solution->root_count = 1;
            solution->root1 = -c / b;
            return QUADRATIC_LINEAR_EQUATION;
        }
    }
    
    double discriminant = b * b - 4 * a * c;
    
    if (discriminant < -epsilon) {
        return QUADRATIC_NO_REAL_ROOTS;
    }
    
    if (fabs(discriminant) <= epsilon) {
        solution->root_count = 1;
        solution->root1 = -b / (2 * a);
    } else {
        solution->root_count = 2;
        double sqrt_discriminant = sqrt(discriminant);
        solution->root1 = (-b - sqrt_discriminant) / (2 * a);
        solution->root2 = (-b + sqrt_discriminant) / (2 * a);
    }
    
    return QUADRATIC_SUCCESS;
}


MultipleStatus check_multiple(int num1, int num2, bool *is_multiple) {
    if (is_multiple == NULL) {
        return MULTIPLE_INVALID_PARAMS;
    }
    
    if (num2 == 0) {
        return MULTIPLE_DIVISION_BY_ZERO;
    }
    
    *is_multiple = (num1 % num2 == 0);
    return MULTIPLE_SUCCESS;
}


TriangleStatus check_right_triangle(double epsilon, double a, double b, double c, bool *is_right_triangle) {
    if (is_right_triangle == NULL) {
        return TRIANGLE_INVALID_PARAMS;
    }
    
    if (a <= 0 || b <= 0 || c <= 0) {
        return TRIANGLE_INVALID_SIDES;
    }
    
    if (a + b <= c || a + c <= b || b + c <= a) {
        return TRIANGLE_NOT_TRIANGLE;
    }
    
    *is_right_triangle = false;
    
    if (fabs(a * a + b * b - c * c) <= epsilon) {
        *is_right_triangle = true;
    }
    else if (fabs(a * a + c * c - b * b) <= epsilon) {
        *is_right_triangle = true;
    }
    else if (fabs(b * b + c * c - a * a) <= epsilon) {
        *is_right_triangle = true;
    }
    
    return TRIANGLE_SUCCESS;
}


QuadraticStatus solve_quadratic_with_permutations(double epsilon, double a, double b, double c, PermutationsResult *result) {
    if (result == NULL) {
        return QUADRATIC_INVALID_PARAMS;
    }
    
    double coefficients[3] = {a, b, c};
    int permutations[6][3] = {
        {0, 1, 2}, {0, 2, 1}, {1, 0, 2},
        {1, 2, 0}, {2, 0, 1}, {2, 1, 0}
    };
    
    for (int i = 0; i < 6; i++) {
        double perm_a = coefficients[permutations[i][0]];
        double perm_b = coefficients[permutations[i][1]];
        double perm_c = coefficients[permutations[i][2]];
        
        result->coefficients[i][0] = perm_a;
        result->coefficients[i][1] = perm_b;
        result->coefficients[i][2] = perm_c;
        
        result->statuses[i] = solve_quadratic_equation(
            epsilon, 
            perm_a, 
            perm_b, 
            perm_c, 
            &result->solutions[i]
        );
    }
    
    return QUADRATIC_SUCCESS;
}

void print_solution(const QuadraticSolution *solution) {
    if (solution == NULL) return;
    
    switch (solution->root_count) {
        case 0:
            printf("No real roots");
            break;
        case 1:
            printf("One real root: %.6f", solution->root1);
            break;
        case 2:
            printf("Two real roots: %.6f and %.6f", solution->root1, solution->root2);
            break;
        default:
            printf("Unexpected number of roots: %d", solution->root_count);
            break;
    }
}

void print_quadratic_status(QuadraticStatus status) {
    switch (status) {
        case QUADRATIC_SUCCESS:
            printf("Quadratic equation solved successfully");
            break;
        case QUADRATIC_INVALID_PARAMS:
            printf("Error: Invalid parameters");
            break;
        case QUADRATIC_NO_REAL_ROOTS:
            printf("No real roots");
            break;
        case QUADRATIC_LINEAR_EQUATION:
            printf("Linear equation");
            break;
        case QUADRATIC_IDENTITY:
            printf("Identity equation (infinite solutions)");
            break;
        default:
            printf("Unknown status");
            break;
    }
}

void print_multiple_result(int num1, int num2, bool is_multiple, MultipleStatus status) {
    switch (status) {
        case MULTIPLE_SUCCESS:
            if (is_multiple) {
                printf("Yes, %d is a multiple of %d", num1, num2);
            } else {
                printf("No, %d is not a multiple of %d", num1, num2);
            }
            break;
        case MULTIPLE_DIVISION_BY_ZERO:
            printf("Error: Division by zero");
            break;
        case MULTIPLE_INVALID_PARAMS:
            printf("Error: Invalid parameters");
            break;
        default:
            printf("Unknown status");
            break;
    }
}

void print_triangle_result(double a, double b, double c, bool is_right_triangle, TriangleStatus status) {
    switch (status) {
        case TRIANGLE_SUCCESS:
            if (is_right_triangle) {
                printf("The sides %.6f, %.6f, %.6f can form a right triangle", a, b, c);
            } else {
                printf("The sides %.6f, %.6f, %.6f cannot form a right triangle", a, b, c);
            }
            break;
        case TRIANGLE_NOT_TRIANGLE:
            printf("Error: These sides cannot form any triangle (violates triangle inequality)");
            break;
        case TRIANGLE_INVALID_SIDES:
            printf("Error: All triangle sides must be positive");
            break;
        case TRIANGLE_INVALID_PARAMS:
            printf("Error: Invalid parameters");
            break;
        default:
            printf("Unknown status");
            break;
    }
}

void print_permutations_result(const PermutationsResult *result) {
    if (result == NULL) return;
    
    printf("Solving quadratic equations with all permutations of coefficients:\n\n");
    
    for (int i = 0; i < 6; i++) {
        printf("Permutation %d: a=%.6f, b=%.6f, c=%.6f\n",
               i + 1,
               result->coefficients[i][0],
               result->coefficients[i][1],
               result->coefficients[i][2]);
        
        printf("Status: ");
        print_quadratic_status(result->statuses[i]);
        printf("\n");
        
        if (result->statuses[i] == QUADRATIC_SUCCESS || 
            result->statuses[i] == QUADRATIC_LINEAR_EQUATION) {
            printf("Solution: ");
            print_solution(&result->solutions[i]);
            printf("\n");
        }
        
        printf("\n");
    }
}