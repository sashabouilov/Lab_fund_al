#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "file_processor.h"

#define MAX_LINE_LENGTH 1024

int process_file(const char *input_filename, const char *output_filename, char operation) {
    if (input_filename == NULL || output_filename == NULL) {
        return PROC_ERR_INVALID_ARG;
    }

    FILE *input = fopen(input_filename, "r");
    if (input == NULL) {
        perror("Error opening input file");
        return PROC_ERR_FILE_OPEN;
    }

    FILE *output = fopen(output_filename, "w");
    if (output == NULL) {
        perror("Error opening output file");
        fclose(input);
        return PROC_ERR_FILE_OPEN;
    }

    char line[MAX_LINE_LENGTH];
    ProcessingStatus status = PROC_SUCCESS;

    ProcessingStatus (*process_func)(const char *, FILE *) = NULL;
    switch (operation) {
        case 'd': process_func = process_line_d; break;
        case 'i': process_func = process_line_i; break;
        case 's': process_func = process_line_s; break;
        case 'a': process_func = process_line_a; break;
        default:
            status = PROC_ERR_UNKNOWN_OPERATION;
            break;
    }

    if (status == PROC_SUCCESS && process_func != NULL) {
        while (fgets(line, sizeof(line), input) != NULL) {
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n') {
                line[len - 1] = '\0';
            }

            status = process_func(line, output);
            if (status != PROC_SUCCESS) {
                break;
            }
        }
    }

    fclose(input);
    fclose(output);

    return (status == PROC_SUCCESS) ? EXIT_SUCCESS : EXIT_FAILURE;
}

ProcessingStatus process_line_d(const char *line, FILE *output) {
    if (line == NULL || output == NULL) {
        return PROC_ERR_INVALID_ARG;
    }

    for (size_t i = 0; line[i] != '\0'; i++) {
        if (!isdigit((unsigned char)line[i])) {
            fputc(line[i], output);
        }
    }
    fputc('\n', output);

    return PROC_SUCCESS;
}

ProcessingStatus process_line_i(const char *line, FILE *output) {
    if (line == NULL || output == NULL) {
        return PROC_ERR_INVALID_ARG;
    }

    int count = 0;
    for (size_t i = 0; line[i] != '\0'; i++) {
        if (isalpha((unsigned char)line[i])) {
            count++;
        }
    }
    fprintf(output, "%d\n", count);

    return PROC_SUCCESS;
}

ProcessingStatus process_line_s(const char *line, FILE *output) {
    if (line == NULL || output == NULL) {
        return PROC_ERR_INVALID_ARG;
    }

    int count = 0;
    for (size_t i = 0; line[i] != '\0'; i++) {
        unsigned char c = line[i];
        if (!isalpha(c) && !isdigit(c) && !isspace(c)) {
            count++;
        }
    }
    fprintf(output, "%d\n", count);

    return PROC_SUCCESS;
}

ProcessingStatus process_line_a(const char *line, FILE *output) {
    if (line == NULL || output == NULL) {
        return PROC_ERR_INVALID_ARG;
    }

    for (size_t i = 0; line[i] != '\0'; i++) {
        unsigned char c = line[i];
        if (isdigit(c)) {
            fputc(c, output);
        } else {
            fprintf(output, "%02X", c);
        }
    }
    fputc('\n', output);

    return PROC_SUCCESS;
}