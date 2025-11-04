#ifndef FINITE_REPRESENTATION_H
#define FINITE_REPRESENTATION_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

int check_finite_representation(int base, double epsilon, int count, ...);

bool is_finite_representation(double number, int base, double epsilon);

#endif