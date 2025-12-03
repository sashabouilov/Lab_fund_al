#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "file_processor.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <flag> <input_file> [output_file]\n", argv[0]);
        fprintf(stderr, "Flags: -d, -i, -s, -a (with optional 'n' for output file)\n");
        return EXIT_FAILURE;
    }

    const char *flag = argv[1];
    const char *input_file = argv[2];
    char *output_file = NULL;

    if (strlen(flag) < 2 || strlen(flag) > 3 || (flag[0] != '-' && flag[0] != '/')) {
        fprintf(stderr, "Invalid flag format: %s\n", flag);
        fprintf(stderr, "Flag must start with '-' or '/' and be 2-3 chars long\n");
        return EXIT_FAILURE;
    }

    char operation = flag[strlen(flag) - 1];
    
    if (operation != 'd' && operation != 'i' && operation != 's' && operation != 'a') {
        fprintf(stderr, "Unknown operation flag: %c\n", operation);
        fprintf(stderr, "Allowed operations: d, i, s, a\n");
        return EXIT_FAILURE;
    }

    int has_n = (strlen(flag) == 3 && flag[1] == 'n');
    
    if (has_n && argc >= 4) {
        output_file = argv[3];
    } else if (!has_n && argc >= 4) {
        fprintf(stderr, "Error: Output file specified without 'n' flag\n");
        fprintf(stderr, "When using flags without 'n', output file is auto-generated\n");
        return EXIT_FAILURE;
    } else {
        const char *prefix = "out_";
        size_t len = strlen(input_file) + strlen(prefix) + 1;
        output_file = (char *)malloc(len);
        if (output_file == NULL) {
            fprintf(stderr, "Memory allocation failed for output file name.\n");
            return EXIT_FAILURE;
        }
        snprintf(output_file, len, "%s%s", prefix, input_file);
    }

    int result = process_file(input_file, output_file, operation);

    if (!has_n && argc < 4) {
        free(output_file);
    }

    if (result != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}