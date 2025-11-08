#include "file_transfer.h"
#include <stdio.h>
#include <string.h>

#define MAX_PATH_LENGTH 256


void print_status_message(StatusCode status) {
    switch (status) {
        case SUCCESS:
            printf("Success: File formatting completed successfully.\n");
            break;
        case ERROR_OPEN_INPUT_FILE:
            printf("Error: Cannot open input file.\n");
            break;
        case ERROR_OPEN_OUTPUT_FILE:
            printf("Error: Cannot open output file.\n");
            break;
        case ERROR_MEMORY_ALLOCATION:
            printf("Error: Memory allocation failed.\n");
            break;
        case ERROR_INVALID_ARGUMENTS:
            printf("Error: Invalid arguments.\n");
            break;
        case ERROR_READING_FILE:
            printf("Error: File reading error.\n");
            break;
        case ERROR_WRITING_FILE:
            printf("Error: File writing error.\n");
            break;
        case ERROR_SAME_FILE_NAMES:
            printf("Error: Input and output file names are the same.\n");
            break;
        default:
            printf("Error: Unknown error.\n");
            break;
    }
}


int main() {
    char input_path[MAX_PATH_LENGTH];
    char output_path[MAX_PATH_LENGTH];
    
    printf("Enter input file path: ");
    if (fgets(input_path, sizeof(input_path), stdin) == NULL) {
        printf("Error: Input error.\n");
        return 1;
    }
    input_path[strcspn(input_path, "\n")] = '\0';
    
    printf("Enter output file path: ");
    if (fgets(output_path, sizeof(output_path), stdin) == NULL) {
        printf("Error: Input error.\n");
        return 1;
    }
    output_path[strcspn(output_path, "\n")] = '\0';
    
    if (strlen(input_path) == 0 || strlen(output_path) == 0) {
        printf("Error: File paths cannot be empty.\n");
        return 1;
    }
    
    StatusCode status = process_file_formatting(input_path, output_path);
    print_status_message(status);
    
    if (status == SUCCESS) {
        FILE* test_file = fopen(output_path, "r");
        if (test_file != NULL) {
            fseek(test_file, 0, SEEK_END);
            long size = ftell(test_file);
            fclose(test_file);
            printf("Output file size: %ld bytes\n", size);
        } else {
            printf("Warning: Could not verify output file\n");
        }
    }
    
    return (status == SUCCESS) ? 0 : 1;
}