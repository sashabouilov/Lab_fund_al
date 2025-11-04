#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stddef.h>

typedef enum {
    STR_SUCCESS = 0,
    STR_NULL_POINTER = 1,
    STR_INVALID_SIZE = 2,
    STR_NOT_FOUND = 3,
    STR_NO_MEMORY = 4,
    STR_INVALID_PARAM = 5
} str_status_t;

void *memchr(const void *str, int c, size_t n);
int memcmp(const void *str1, const void *str2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *str, int c, size_t n);
char *strncat(char *dest, const char *src, size_t n);
char *strchr(const char *str, int c);
int strncmp(const char *str1, const char *str2, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
size_t strcspn(const char *str1, const char *str2);
char *strerror(int errnum);
size_t strlen(const char *str);
char *strpbrk(const char *str1, const char *str2);
char *strrchr(const char *str, int c);
char *strstr(const char *haystack, const char *needle);
char *strtok(char *str, const char *delim);

str_status_t get_last_error(void);
void reset_error(void);

#endif 