#include <stdio.h>
#include <stdlib.h>
#include "constants_calc.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <epsilon>\n", argv[0]);
        return EXIT_FAILURE;
    }

    double epsilon = 0.0;
    if (!parse_double(argv[1], &epsilon) || epsilon <= 0.0) {
        fprintf(stderr, "Error: epsilon must be a positive number.\n");
        return EXIT_FAILURE;
    }

    compute_and_print(epsilon);

    return EXIT_SUCCESS;
}