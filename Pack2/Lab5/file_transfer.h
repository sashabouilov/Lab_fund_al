#ifndef FILE_TRANSFER_H
#define FILE_TRANSFER_H

#include <stddef.h>

typedef enum {
    SUCCESS = 0,
    ERROR_OPEN_INPUT_FILE,
    ERROR_OPEN_OUTPUT_FILE,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_ARGUMENTS,
    ERROR_READING_FILE,
    ERROR_WRITING_FILE,
    ERROR_SAME_FILE_NAMES
} StatusCode;

StatusCode process_file_formatting(const char* input_path, const char* output_path);

#endif