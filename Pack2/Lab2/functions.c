#include "functions.h"
#include <stdio.h>

static str_status_t last_error = STR_SUCCESS;

static char *strtok_save = NULL;

str_status_t get_last_error(void) {
    return last_error;
}

void reset_error(void) {
    last_error = STR_SUCCESS;
}

static void set_error(str_status_t error) {
    last_error = error;
}

void *memchr(const void *str, int c, size_t n) {
    reset_error();
    
    if (str == NULL) {
        set_error(STR_NULL_POINTER);
        return NULL;
    }
    
    const unsigned char *ptr = (const unsigned char *)str;
    unsigned char uc = (unsigned char)c;
    
    for (size_t i = 0; i < n; i++) {
        if (ptr[i] == uc) {
            return (void *)(ptr + i);
        }
    }
    
    set_error(STR_NOT_FOUND);
    return NULL;
}

int memcmp(const void *str1, const void *str2, size_t n) {
    reset_error();
    
    if (str1 == NULL || str2 == NULL) {
        set_error(STR_NULL_POINTER);
        return 0;
    }
    
    const unsigned char *p1 = (const unsigned char *)str1;
    const unsigned char *p2 = (const unsigned char *)str2;
    
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }
    
    return 0;
}

void *memcpy(void *dest, const void *src, size_t n) {
    reset_error();
    
    if (dest == NULL || src == NULL) {
        set_error(STR_NULL_POINTER);
        return NULL;
    }
    
    if (src < dest && (char*)src + n > (char*)dest) {
        unsigned char *d = (unsigned char *)dest + n - 1;
        const unsigned char *s = (const unsigned char *)src + n - 1;
        
        for (size_t i = 0; i < n; i++) {
            *d-- = *s--;
        }
    } else {
        unsigned char *d = (unsigned char *)dest;
        const unsigned char *s = (const unsigned char *)src;
        
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    }
    
    return dest;
}

void *memset(void *str, int c, size_t n) {
    reset_error();
    
    if (str == NULL) {
        set_error(STR_NULL_POINTER);
        return NULL;
    }
    
    unsigned char *ptr = (unsigned char *)str;
    unsigned char uc = (unsigned char)c;
    
    for (size_t i = 0; i < n; i++) {
        ptr[i] = uc;
    }
    
    return str;
}

char *strncat(char *dest, const char *src, size_t n) {
    reset_error();
    
    if (dest == NULL || src == NULL) {
        set_error(STR_NULL_POINTER);
        return NULL;
    }
    
    char *dest_end = dest;
    while (*dest_end != '\0') {
        dest_end++;
    }
    
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest_end[i] = src[i];
    }
    
    dest_end[i] = '\0';
    
    return dest;
}

char *strchr(const char *str, int c) {
    reset_error();
    
    if (str == NULL) {
        set_error(STR_NULL_POINTER);
        return NULL;
    }
    
    unsigned char uc = (unsigned char)c;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == uc) {
            return (char *)(str + i);
        }
    }
    
    if (uc == '\0') {
        return (char *)str;
    }
    
    set_error(STR_NOT_FOUND);
    return NULL;
}

int strncmp(const char *str1, const char *str2, size_t n) {
    reset_error();
    
    if (str1 == NULL || str2 == NULL) {
        set_error(STR_NULL_POINTER);
        return 0;
    }
    
    for (size_t i = 0; i < n; i++) {
        unsigned char c1 = (unsigned char)str1[i];
        unsigned char c2 = (unsigned char)str2[i];
        
        if (c1 != c2) {
            return c1 < c2 ? -1 : 1;
        }
        if (c1 == '\0') {
            break;
        }
    }
    
    return 0;
}

char *strncpy(char *dest, const char *src, size_t n) {
    reset_error();
    
    if (dest == NULL || src == NULL) {
        set_error(STR_NULL_POINTER);
        return NULL;
    }
    
    size_t i;
    
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    
    return dest;
}

size_t strcspn(const char *str1, const char *str2) {
    reset_error();
    
    if (str1 == NULL || str2 == NULL) {
        set_error(STR_NULL_POINTER);
        return 0;
    }
    
    size_t count = 0;
    for (; str1[count] != '\0'; count++) {
        for (size_t j = 0; str2[j] != '\0'; j++) {
            if (str1[count] == str2[j]) {
                return count;
            }
        }
    }
    
    return count;
}

char *strerror(int errnum) {
    reset_error();
    
    static const char *error_messages[] = {
        "No error",
        "Invalid function",
        "File not found",
        "Path not found",
        "Too many open files",
        "Access denied",
        "Invalid handle",
        "Memory control blocks destroyed",
        "Insufficient memory",
        "Invalid memory block address",
        "Invalid environment",
        "Invalid format",
        "Invalid access code",
        "Invalid data",
        "Unknown error"
    };
    
    const size_t max_errors = sizeof(error_messages) / sizeof(error_messages[0]) - 1;
    
    if (errnum < 0 || (size_t)errnum > max_errors) {
        set_error(STR_INVALID_PARAM);
        return (char *)error_messages[max_errors];
    }
    
    return (char *)error_messages[errnum];
}

size_t strlen(const char *str) {
    reset_error();
    
    if (str == NULL) {
        set_error(STR_NULL_POINTER);
        return 0;
    }
    
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    
    return length;
}

char *strpbrk(const char *str1, const char *str2) {
    reset_error();
    
    if (str1 == NULL || str2 == NULL) {
        set_error(STR_NULL_POINTER);
        return NULL;
    }
    
    for (size_t i = 0; str1[i] != '\0'; i++) {
        for (size_t j = 0; str2[j] != '\0'; j++) {
            if (str1[i] == str2[j]) {
                return (char *)(str1 + i);
            }
        }
    }
    
    set_error(STR_NOT_FOUND);
    return NULL;
}

char *strrchr(const char *str, int c) {
    reset_error();
    
    if (str == NULL) {
        set_error(STR_NULL_POINTER);
        return NULL;
    }
    
    unsigned char uc = (unsigned char)c;
    const char *last_occurrence = NULL;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == uc) {
            last_occurrence = str + i;
        }
    }
    
    if (uc == '\0') {
        last_occurrence = str;
    }
    
    if (last_occurrence == NULL) {
        set_error(STR_NOT_FOUND);
    }
    
    return (char *)last_occurrence;
}

char *strstr(const char *haystack, const char *needle) {
    reset_error();
    
    if (haystack == NULL || needle == NULL) {
        set_error(STR_NULL_POINTER);
        return NULL;
    }
    
    if (needle[0] == '\0') {
        return (char *)haystack;
    }
    
    for (size_t i = 0; haystack[i] != '\0'; i++) {
        size_t j = 0;
        
        while (needle[j] != '\0' && haystack[i + j] != '\0' && 
               haystack[i + j] == needle[j]) {
            j++;
        }
        
        if (needle[j] == '\0') {
            return (char *)(haystack + i);
        }
        
        if (haystack[i + j] == '\0') {
            break;
        }
    }
    
    set_error(STR_NOT_FOUND);
    return NULL;
}

char *strtok(char *str, const char *delim) {
    reset_error();
    
    if (delim == NULL) {
        set_error(STR_NULL_POINTER);
        return NULL;
    }
    
    char *current_str = (str != NULL) ? str : strtok_save;
    
    if (current_str == NULL) {
        return NULL;
    }
    
    while (*current_str != '\0') {
        const char *d = delim;
        int is_delim = 0;
        
        while (*d != '\0') {
            if (*current_str == *d) {
                is_delim = 1;
                break;
            }
            d++;
        }
        
        if (!is_delim) {
            break;
        }
        current_str++;
    }
    
    if (*current_str == '\0') {
        strtok_save = NULL;
        return NULL;
    }
    
    char *token_start = current_str;
    
    while (*current_str != '\0') {
        const char *d = delim;
        int is_delim = 0;
        
        while (*d != '\0') {
            if (*current_str == *d) {
                is_delim = 1;
                break;
            }
            d++;
        }
        
        if (is_delim) {
            break;
        }
        current_str++;
    }
    
    if (*current_str == '\0') {
        strtok_save = NULL;
    } else {
        *current_str = '\0';
        strtok_save = current_str + 1;
    }
    
    return token_start;
}