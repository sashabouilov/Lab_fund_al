#include "file_transfer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TARGET_LENGTH 80
#define INITIAL_BUFFER_SIZE 1024
#define BUFFER_GROWTH_FACTOR 2


int are_file_names_equal(const char* path1, const char* path2) {
    if (path1 == NULL || path2 == NULL) return 0;

    const char separator = '\\';
    
    const char* name1 = strrchr(path1, separator);
    const char* name2 = strrchr(path2, separator);
    
    name1 = (name1 == NULL) ? path1 : name1 + 1;
    name2 = (name2 == NULL) ? path2 : name2 + 1;
    
    return strcmp(name1, name2) == 0;
}


char* read_line(FILE* file, StatusCode* result) {
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    size_t length = 0;
    char* buffer = malloc(buffer_size);
    
    if (buffer == NULL) {
        *result = ERROR_MEMORY_ALLOCATION;
        return NULL;
    }
    
    while (fgets(buffer + length, buffer_size - length, file) != NULL) {
        length += strlen(buffer + length);
        
        if (length > 0 && buffer[length - 1] == '\n') {
            break;
        }
        
        if (length + 1 >= buffer_size) {
            buffer_size *= BUFFER_GROWTH_FACTOR;
            char* new_buffer = realloc(buffer, buffer_size);
            if (new_buffer == NULL) {
                free(buffer);
                *result = ERROR_MEMORY_ALLOCATION;
                return NULL;
            }
            buffer = new_buffer;
        }
    }
    
    if (length == 0 && feof(file)) {
        free(buffer);
        return NULL;
    }
    
    while (length > 0 && (buffer[length - 1] == '\n' || buffer[length - 1] == '\r')) {
        buffer[--length] = '\0';
    }
    
    return buffer;
}


size_t count_utf8_chars(const char* str) {
    size_t count = 0;
    unsigned char* p = (unsigned char*)str;
    
    while (*p) {
        if (*p < 128) {
            count++;
            p++;
        } else if ((*p & 0xE0) == 0xC0) {
            count++;
            p += 2;
        } else if ((*p & 0xF0) == 0xE0) {
            count++;
            p += 3;
        } else if ((*p & 0xF8) == 0xF0) {
            count++;
            p += 4;
        } else {
            p++;
        }
    }
    return count;
}


size_t get_utf8_string_length(const char* str) {
    return count_utf8_chars(str);
}


const char* utf8_char_at(const char* str, size_t n) {
    const char* p = str;
    size_t count = 0;
    
    while (*p && count < n) {
        if ((unsigned char)*p < 128) {
            p++;
        } else if ((*p & 0xE0) == 0xC0) {
            p += 2;
        } else if ((*p & 0xF0) == 0xE0) {
            p += 3;
        } else if ((*p & 0xF8) == 0xF0) {
            p += 4;
        } else {
            p++;
        }
        count++;
    }
    return p;
}


size_t copy_utf8_chars(const char* src, char* dest, size_t max_chars) {
    const char* p = src;
    char* d = dest;
    size_t chars_copied = 0;
    
    while (*p && chars_copied < max_chars) {
        size_t char_len;
        
        if ((unsigned char)*p < 128) {
            char_len = 1;
        } else if ((*p & 0xE0) == 0xC0) {
            char_len = 2;
        } else if ((*p & 0xF0) == 0xE0) {
            char_len = 3;
        } else if ((*p & 0xF8) == 0xF0) {
            char_len = 4;
        } else {
            char_len = 1;
        }
        
        for (size_t i = 0; i < char_len && *p; i++) {
            *d++ = *p++;
        }
        chars_copied++;
    }
    
    *d = '\0';
    return chars_copied;
}


StatusCode format_long_line(const char* line, FILE* output_file) {
    size_t utf8_len = get_utf8_string_length(line);
    
    if (utf8_len <= TARGET_LENGTH) {
        if (fprintf(output_file, "%s\n", line) < 0) {
            return ERROR_WRITING_FILE;
        }
        return SUCCESS;
    }
    
    const char* current_pos = line;
    size_t remaining_chars = utf8_len;
    
    while (remaining_chars > 0) {
        size_t take_chars = (remaining_chars > TARGET_LENGTH) ? TARGET_LENGTH : remaining_chars;
        
        char buffer[TARGET_LENGTH * 4 + 1];
        copy_utf8_chars(current_pos, buffer, take_chars);
        
        if (fprintf(output_file, "%s\n", buffer) < 0) {
            return ERROR_WRITING_FILE;
        }
        
        current_pos = utf8_char_at(current_pos, take_chars);
        remaining_chars -= take_chars;
    }
    
    return SUCCESS;
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
    
    StatusCode result = SUCCESS;
    
    while (result == SUCCESS) {
        char* line = read_line(input_file, &result);
        if (line == NULL) {
            break;
        }
        
        size_t byte_len = strlen(line);
        size_t utf8_len = get_utf8_string_length(line);
        
        printf("Debug: Bytes: %zu, UTF-8 chars: %zu\n", byte_len, utf8_len);
        printf("Debug: Line: '%s'\n", line);
        
        if (byte_len == 0) {
            if (fprintf(output_file, "\n") < 0) {
                result = ERROR_WRITING_FILE;
            }
        } else if (utf8_len <= TARGET_LENGTH) {
            printf("Debug: Line is short (%zu UTF-8 chars), copying as is\n", utf8_len);
            if (fprintf(output_file, "%s\n", line) < 0) {
                result = ERROR_WRITING_FILE;
            }
        } else {
            printf("Debug: Line is long (%zu UTF-8 chars), formatting\n", utf8_len);
            result = format_long_line(line, output_file);
        }
        
        free(line);
        
        if (result != SUCCESS) {
            break;
        }
    }
    
    if (result == SUCCESS && ferror(input_file)) {
        result = ERROR_READING_FILE;
    }
    
    fclose(input_file);
    fclose(output_file);
    
    return result;
}