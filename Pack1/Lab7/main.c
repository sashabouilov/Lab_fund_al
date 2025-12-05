#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "number_utils.h"

#define BUFFER_SIZE 1024

typedef enum {
    ARG_OK = 0,
    ARG_INVALID_COUNT,
    ARG_FILE_OPEN_ERROR
} ArgStatus;

ArgStatus process_args(int argc, char **argv, FILE **in, FILE **out) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return ARG_INVALID_COUNT;
    }
    
    *in = fopen(argv[1], "r");
    if (!*in) {
        perror("Error opening input file");
        return ARG_FILE_OPEN_ERROR;
    }
    
    *out = fopen(argv[2], "w");
    if (!*out) {
        perror("Error opening output file");
        fclose(*in);
        return ARG_FILE_OPEN_ERROR;
    }
    
    return ARG_OK;
}

int is_delimiter(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int main(int argc, char **argv) {
    FILE *in = NULL, *out = NULL;
    ArgStatus arg_status = process_args(argc, argv, &in, &out);
    if (arg_status != ARG_OK) {
        return EXIT_FAILURE;
    }
    
    char buffer[BUFFER_SIZE];
    size_t buf_pos = 0;
    
    while (1) {
        int ch = fgetc(in);
        if (ch == EOF || is_delimiter(ch)) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                
                char *cleaned = NULL;
                NumberStatus status = remove_leading_zeros(buffer, &cleaned);
                if (status != OK) {
                    free(cleaned);
                    continue;
                }
                
                int base = 0;
                status = min_base_for_number(cleaned, &base);
                if (status != OK) {
                    free(cleaned);
                    continue;
                }
                
                long long decimal = 0;
                status = str_to_ll(cleaned, base, &decimal);
                if (status != OK) {
                    free(cleaned);
                    continue;
                }
                
                fprintf(out, "%s %d %lld\n", cleaned, base, decimal);
                free(cleaned);
                buf_pos = 0;
            }
            if (ch == EOF) break;
        } else {
            if (buf_pos < BUFFER_SIZE - 1) {
                buffer[buf_pos++] = (char)ch;
            }
        }
    }
    
    fclose(in);
    fclose(out);
    
    return EXIT_SUCCESS;
}