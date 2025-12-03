#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <stdio.h>

typedef enum {
    PROC_SUCCESS = 0,
    PROC_ERR_FILE_OPEN,
    PROC_ERR_MEMORY,
    PROC_ERR_INVALID_ARG,
    PROC_ERR_UNKNOWN_OPERATION
} ProcessingStatus;

int process_file(const char *input_filename, const char *output_filename, char operation);

ProcessingStatus process_line_d(const char *line, FILE *output);
ProcessingStatus process_line_i(const char *line, FILE *output);
ProcessingStatus process_line_s(const char *line, FILE *output);
ProcessingStatus process_line_a(const char *line, FILE *output);

#endif