#include "brackets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


char* create_large_brackets_string(int depth) {
    size_t length = (size_t)depth * 2 * 4 + 1;
    char* str = (char*)malloc(length);
    if (str == NULL) return NULL;
    
    int pos = 0;
    for (int i = 0; i < depth; i++) {
        str[pos++] = '(';
        str[pos++] = '[';
        str[pos++] = '{';
        str[pos++] = '<';
    }
    for (int i = 0; i < depth; i++) {
        str[pos++] = '>';
        str[pos++] = '}';
        str[pos++] = ']';
        str[pos++] = ')';
    }
    str[pos] = '\0';
    return str;
}


void run_tests() {
    printf("Running tests...\n");
    {
        const char *test1 = "(a + b) * [c - d] / {e % f} + <g & h>";
        BracketsStatus result = check_brackets(test1);
        printf("Test 1: Simple brackets - %s\n", get_brackets_status_message(result));
        assert(result == BRACKETS_OK);
    }
    

    {
        const char *test2 = "({[<>]})";
        BracketsStatus result = check_brackets(test2);
        printf("Test 2: Nested brackets - %s\n", get_brackets_status_message(result));
        assert(result == BRACKETS_OK);
    }
    

    {
        const char *test3 = "(a + b] * c";
        BracketsStatus result = check_brackets(test3);
        printf("Test 3: Unbalanced brackets - %s\n", get_brackets_status_message(result));
        assert(result == BRACKETS_UNBALANCED);
    }
    

    {
        const char *test4 = "a + b) * c";
        BracketsStatus result = check_brackets(test4);
        printf("Test 4: Extra Closing - %s\n", get_brackets_status_message(result));
        assert(result == BRACKETS_UNBALANCED);
    }
    

    {
        const char *test5 = "(a + b * c";
        BracketsStatus result = check_brackets(test5);
        printf("Test 5: An extra Opener - %s\n", get_brackets_status_message(result));
        assert(result == BRACKETS_UNBALANCED);
    }
    

    {
        const char *test6 = "";
        BracketsStatus result = check_brackets(test6);
        printf("Test 6: Empty string - %s\n", get_brackets_status_message(result));
        assert(result == BRACKETS_EMPTY_STRING);
    }
    

    {
        BracketsStatus result = check_brackets(NULL);
        printf("Test 7: NULL pointer - %s\n", get_brackets_status_message(result));
        assert(result == BRACKETS_NULL_PTR);
    }
    

    {
        const char *test8 = "Hello World 123";
        BracketsStatus result = check_brackets(test8);
        printf("Test 8: Without brackets - %s\n", get_brackets_status_message(result));
        assert(result == BRACKETS_OK);
    }
    

    {
        const char *test9 = "if (x) { arr[i] = (a + b) * c; }";
        BracketsStatus result = check_brackets(test9);
        printf("Test 9: Complex Expression - %s\n", get_brackets_status_message(result));
        assert(result == BRACKETS_OK);
    }
    

    {
        const char *test10 = "vector<int> arr = {1, 2, 3};";
        BracketsStatus result = check_brackets(test10);
        printf("Test 10: Angle brackets as type - %s\n", get_brackets_status_message(result));
        assert(result == BRACKETS_OK);
    }
    

    {
        const char *test11 = "({()[]})<>{{}}[][]()";
        BracketsStatus result = check_brackets(test11);
        printf("Test 11: Mixed Brackets - %s\n", get_brackets_status_message(result));
        assert(result == BRACKETS_OK);
    }
    

    {
        char* test12 = create_large_brackets_string(1000);
        if (test12 != NULL) {
            BracketsStatus result = check_brackets(test12);
            printf("Test 12: Large Depth (1000 levels) - %s\n", get_brackets_status_message(result));
            assert(result == BRACKETS_OK);
            free(test12);
        } else {
            printf("Test 12: Skipped (memory error)\n");
        }
    }
    
    printf("All tests passed successfully!\n");
}