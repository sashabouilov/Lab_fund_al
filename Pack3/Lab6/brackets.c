#include "brackets.h"
#include <stdlib.h>
#include <stddef.h>

#define INITIAL_STACK_CAPACITY 100


typedef struct {
    char *data;
    int top;
    int capacity;
} CharStack;


int stack_init(CharStack *stack) {
    stack->data = (char*)malloc(INITIAL_STACK_CAPACITY * sizeof(char));
    if (stack->data == NULL) return 0;
    stack->top = -1;
    stack->capacity = INITIAL_STACK_CAPACITY;
    return 1;
}


void stack_free(CharStack *stack) {
    if (stack->data != NULL) {
        free(stack->data);
    }
}


int stack_is_empty(const CharStack *stack) {
    return stack->top == -1;
}


int stack_resize(CharStack *stack, int new_capacity) {
    char *new_data = (char*)realloc(stack->data, new_capacity * sizeof(char));
    if (new_data == NULL) return 0;
    stack->data = new_data;
    stack->capacity = new_capacity;
    return 1;
}


int stack_push(CharStack *stack, char c) {
    if (stack->top >= stack->capacity - 1) {
        if (!stack_resize(stack, stack->capacity * 2)) return 0;
    }
    stack->data[++stack->top] = c;
    return 1;
}


int stack_pop(CharStack *stack, char *c) {
    if (stack_is_empty(stack)) return 0;
    *c = stack->data[stack->top--];
    return 1;
}


int is_opening_bracket(char c) {
    return c == '(' || c == '[' || c == '{' || c == '<';
}


int is_closing_bracket(char c) {
    return c == ')' || c == ']' || c == '}' || c == '>';
}


char get_matching_opening(char closing) {
    switch (closing) {
        case ')': return '(';
        case ']': return '[';
        case '}': return '{';
        case '>': return '<';
        default: return '\0';
    }
}


BracketsStatus check_brackets(const char *str) {
    if (str == NULL) return BRACKETS_NULL_PTR;
    if (str[0] == '\0') return BRACKETS_EMPTY_STRING;
    
    CharStack stack;
    if (!stack_init(&stack)) return BRACKETS_MEMORY_ERROR;
    
    BracketsStatus final_status = BRACKETS_OK;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        char current = str[i];
        
        if (is_opening_bracket(current)) {
            if (!stack_push(&stack, current)) {
                final_status = BRACKETS_MEMORY_ERROR;
                break;
            }
        } 
        else if (is_closing_bracket(current)) {
            char top_bracket;
            
            if (!stack_pop(&stack, &top_bracket)) {
                final_status = BRACKETS_UNBALANCED;
                break;
            }
            
            char expected_opening = get_matching_opening(current);
            if (top_bracket != expected_opening) {
                final_status = BRACKETS_UNBALANCED;
                break;
            }
        }
    }
    
    if (final_status == BRACKETS_OK && !stack_is_empty(&stack)) {
        final_status = BRACKETS_UNBALANCED;
    }
    
    stack_free(&stack);
    return final_status;
}


const char* get_brackets_status_message(BracketsStatus status) {
    switch (status) {
        case BRACKETS_OK: return "The brackets are balanced correctly";
        case BRACKETS_UNBALANCED: return "The brackets are not balanced";
        case BRACKETS_NULL_PTR: return "Error: A null pointer was passed";
        case BRACKETS_EMPTY_STRING: return "Error: an empty string was passed";
        case BRACKETS_MEMORY_ERROR: return "Memory allocation error";
        default: return "Unknown status";
    }
}