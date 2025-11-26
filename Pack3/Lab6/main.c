#include "brackets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* read_input_string(void) {
    size_t capacity = 100;
    size_t length = 0;
    char* buffer = (char*)malloc(capacity * sizeof(char));
    
    if (buffer == NULL) {
        return NULL;
    }
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char* new_buffer = (char*)realloc(buffer, capacity);
            if (new_buffer == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
        
        buffer[length++] = (char)c;
    }
    
    buffer[length] = '\0';
    return buffer;
}


int main() {
    printf("Checking the balance of brackets\n");
    printf("Supported brackets: (), [], {}, <>\n");
    printf("Note: < and > are treated as brackets, not as operators\n");
    printf("Enter a string to test (or 'test' to run the tests):\n");
    
    char* input = read_input_string();
    
    if (input == NULL) {
        printf("Error allocating memory for input.\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        printf("Empty string entered.\n");
        free(input);
        return 0;
    }
    
    if (strcmp(input, "test") == 0) {
        free(input);
        extern void run_tests();
        run_tests();
        return 0;
    }
    
    BracketsStatus result = check_brackets(input);
    
    printf("\nResult: %s\n", get_brackets_status_message(result));
    printf("String length: %zu characters\n", strlen(input));
    
    if (strlen(input) > 100) {
        printf("String beginning: \"%.100s...\"\n", input);
    } else {
        printf("String: \"%s\"\n", input);
    }
    
    if (result == BRACKETS_OK) {
        printf("Brackets are placed correctly!\n");
    } else {
        printf("Errors in bracket placement found!\n");
    }
    
    free(input);
    return 0;
}