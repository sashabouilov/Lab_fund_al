#include "operations.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


void swap(double *a, double *b) {
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


void print_solution(const QuadraticSolution *solution) {
    if (solution == NULL) return;
    
    switch (solution->root_count) {
        case 0:
            printf("No real roots\n");
            break;
        case 1:
            printf("One real root: %.6f\n", solution->root1);
            break;
        case 2:
            printf("Two real roots: %.6f and %.6f\n", solution->root1, solution->root2);
            break;
        default:
            printf("Unexpected number of roots: %d\n", solution->root_count);
            break;
    }
}


void solve_quadratic_with_permutations(double epsilon, double a, double b, double c) {
    double coefficients[3] = {a, b, c};
    int permutations[6][3] = {
        {0, 1, 2}, {0, 2, 1}, {1, 0, 2},
        {1, 2, 0}, {2, 0, 1}, {2, 1, 0}
    };
    
    printf("Solving quadratic equations with all permutations of coefficients (a, b, c):\n");
    printf("Original coefficients: a=%.6f, b=%.6f, c=%.6f\n", a, b, c);
    printf("Epsilon: %.6f\n\n", epsilon);
    
    for (int i = 0; i < 6; i++) {
        double perm_a = coefficients[permutations[i][0]];
        double perm_b = coefficients[permutations[i][1]];
        double perm_c = coefficients[permutations[i][2]];
        
        printf("Permutation %d: a=%.6f, b=%.6f, c=%.6f\n", 
               i + 1, perm_a, perm_b, perm_c);
        
        QuadraticSolution solution;
        QuadraticStatus status = solve_quadratic_equation(epsilon, perm_a, perm_b, perm_c, &solution);
        
        switch (status) {
            case QUADRATIC_SUCCESS:
                print_solution(&solution);
                break;
            case QUADRATIC_NO_REAL_ROOTS:
                printf("No real roots\n");
                break;
            case QUADRATIC_LINEAR_EQUATION:
                printf("Linear equation: ");
                print_solution(&solution);
                break;
            case QUADRATIC_IDENTITY:
                printf("Identity equation: infinite solutions\n");
                break;
            case QUADRATIC_INVALID_PARAMS:
                printf("Invalid parameters\n");
                break;
            default:
                printf("Unknown status\n");
                break;
        }
        printf("\n");
    }
}


void check_multiple(int num1, int num2) {
    printf("Checking if %d is a multiple of %d: ", num1, num2);
    
    if (num2 == 0) {
        printf("Error: Division by zero\n");
        return;
    }
    
    if (num1 % num2 == 0) {
        printf("Yes, %d is a multiple of %d\n", num1, num2);
    } else {
        printf("No, %d is not a multiple of %d\n", num1, num2);
    }
}


void check_right_triangle(double epsilon, double a, double b, double c) {
    printf("Checking if sides %.6f, %.6f, %.6f can form a right triangle\n", a, b, c);
    
    if (a + b <= c || a + c <= b || b + c <= a) {
        printf("Error: These sides cannot form any triangle (violates triangle inequality)\n");
        return;
    }
    
    int is_right_triangle = 0;
    
    if (fabs(a * a + b * b - c * c) <= epsilon) {
        printf("Right triangle: %.6f^2 + %.6f^2 = %.6f^2\n", a, b, c);
        is_right_triangle = 1;
    }
    else if (fabs(a * a + c * c - b * b) <= epsilon) {
        printf("Right triangle: %.6f^2 + %.6f^2 = %.6f^2\n", a, c, b);
        is_right_triangle = 1;
    }
    else if (fabs(b * b + c * c - a * a) <= epsilon) {
        printf("Right triangle: %.6f^2 + %.6f^2 = %.6f^2\n", b, c, a);
        is_right_triangle = 1;
    }
    
    if (!is_right_triangle) {
        printf("Not a right triangle\n");
    }
}