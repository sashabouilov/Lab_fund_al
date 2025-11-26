#ifndef BRACKETS_H
#define BRACKETS_H

#include <stddef.h>

typedef enum {
    BRACKETS_OK = 0,
    BRACKETS_UNBALANCED = 1,
    BRACKETS_NULL_PTR = 2,
    BRACKETS_EMPTY_STRING = 3,
    BRACKETS_MEMORY_ERROR = 4
} BracketsStatus;

BracketsStatus check_brackets(const char *str);

const char* get_brackets_status_message(BracketsStatus status);

#endif