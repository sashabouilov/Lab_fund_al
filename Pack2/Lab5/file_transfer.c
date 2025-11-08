#include "file_transfer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TARGET_LENGTH 80
#define MAX_LINE_LENGTH 1024


static int are_file_names_equal(const char* path1, const char* path2) {
    if (path1 == NULL || path2 == NULL) return 0;

    const char separator = '\\';
    
    const char* name1 = strrchr(path1, separator);
    const char* name2 = strrchr(path2, separator);
    
    name1 = (name1 == NULL) ? path1 : name1 + 1;
    name2 = (name2 == NULL) ? path2 : name2 + 1;
    
    return strcmp(name1, name2) == 0;
}


StatusCode process_file_formatting(const char* input_path, const char* output_path) {
    if (input_path == NULL || output_path == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    
    if (are_file_names_equal(input_path, output_path)) {
        return ERROR_SAME_FILE_NAMES;
    }
    
    FILE* input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        return ERROR_OPEN_INPUT_FILE;
    }
    
    FILE* output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        fclose(input_file);
        return ERROR_OPEN_OUTPUT_FILE;
    }
    
    char line[MAX_LINE_LENGTH];
    StatusCode result = SUCCESS;
    
    while (fgets(line, sizeof(line), input_file) != NULL) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[--len] = '\0';
        }
        
        if (len == 0) {
            if (fprintf(output_file, "\n") < 0) {
                result = ERROR_WRITING_FILE;
                break;
            }
            continue;
        }
        
        if (len <= TARGET_LENGTH) {
            if (fprintf(output_file, "%s\n", line) < 0) {
                result = ERROR_WRITING_FILE;
                break;
            }
            continue;
        }
        
        const char* current_pos = line;
        
        while (*current_pos != '\0') {
            while (*current_pos == ' ') current_pos++;
            if (*current_pos == '\0') break;
            
            const char* line_end = current_pos + TARGET_LENGTH;
            if (line_end > line + len) {
                line_end = line + len;
            }
            
            const char* break_pos = line_end;
            while (break_pos > current_pos && *break_pos != ' ') {
                break_pos--;
            }
            
            if (break_pos == current_pos) {
                break_pos = line_end;
            }
            
            size_t line_len = break_pos - current_pos;
            
            for (size_t i = 0; i < line_len; i++) {
                if (fputc(current_pos[i], output_file) == EOF) {
                    result = ERROR_WRITING_FILE;
                    break;
                }
            }
            if (fputc('\n', output_file) == EOF) {
                result = ERROR_WRITING_FILE;
                break;
            }
            
            current_pos = break_pos;
            
            if (*current_pos == ' ') {
                current_pos++;
            }
        }
        
        if (result != SUCCESS) break;
    }
    
    if (result == SUCCESS && ferror(input_file)) {
        result = ERROR_READING_FILE;
    }
    
    fclose(input_file);
    fclose(output_file);
    
    return result;
}