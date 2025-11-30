#include "functions.h"


void *memchr(const void *str, int c, size_t n) {
    if (!str) return NULL;
    const unsigned char *s = (const unsigned char*)str;
    unsigned char uc = (unsigned char)c;

    for(size_t i = 0; i < n; ++i) {
        if (s[i] == uc) return (void *)(s + i);
    }

    return NULL;
}


int memcmp(const void *str1, const void *str2, size_t n) {
    if (str1 == NULL && str2 == NULL) return 0;
    if (str1 == NULL) return -1; 
    if (str2 == NULL) return 1;

    const unsigned char *s1 = (const unsigned char*)str1;
    const unsigned char *s2 = (const unsigned char*)str2;

    for (size_t i = 0; i < n; ++i) {
        if (s1[i] != s2[i]) {
            if (s1[i] < s2[i]) return -1;
            else return 1;
        }
    }
    return 0;
}


void *memcpy(void *dest, const void *src, size_t n) {
    if (!dest || !src) return NULL;
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;

    for(size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }

    return dest;
}


void *memset(void *str, int c, size_t n) {
    if (!str) return NULL;
    unsigned char *s = (unsigned char*)str;
    unsigned char uc = (unsigned char)c;

    for(size_t i = 0; i < n; ++i) {
        s[i] = uc;
    }
    return str;
}


char *strncat(char *dest, const char *src, size_t n) {
    if (!dest || !src) return NULL; 
    char *tmp = dest;
    while(*dest) {
        dest++;
    }
    
    while (n-- && *src) {
        *dest = *src;
        src++;
        dest++;
    }

    *dest = '\0';
    return tmp;
}


char *strchr(const char *str, int c) {
    if (!str) return NULL;
    unsigned char uc = (unsigned char)c;

    while (*str) {
        if ((unsigned char)(*str) == uc) return (char *)str;
        str++;
    }

    if (uc == '\0') return (char *)str;

    return NULL;
}


int strncmp(const char *str1, const char *str2, size_t n) {
    if (str1 == NULL && str2 == NULL) return 0;
    if (str1 == NULL) return -1;
    if (str2 == NULL) return 1;

    for (size_t i = 0; i < n; ++i) {
        if (str1[i] != str2[i]) {
            if (str1[i] < str2[i]) return -1;
            else return 1;
        }
    }

    return 0;
}


char *strncpy(char *dest, const char *src, size_t n) {
    if (!dest || !src) return NULL;
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; ++i) {
        dest[i] = src[i];
    }
    for (; i < n; ++i) {
        dest[i] = '\0'; 
    }

    return dest;
}


size_t strcspn(const char *str1, const char *str2) {
    if (!str1 || !str2) return 0;
    const char *p = str1;
    while (*p) {
        const char *q = str2;
        while (*q) {
            if (*p == *q) return (size_t)(p - str1);
            q++;
        }
        p++;
    }
    
    return (size_t)(p - str1);
}


size_t strlen(const char *str) {
    if (!str) return 0;
    size_t count = 0;
    while (*str) {
        count++;
        str++;
    }
    return count;
}


char *strpbrk(const char *str1, const char *str2) {
    if (!str1 || !str2) return NULL;
    const char *p = str1;
    while (*p) {
        const char *q = str2;
        while (*q) {
            if (*p == *q) return (char *)p;
            q++;
        }
        p++;
    }
    return NULL;
}


char *strrchr(const char *str, int c) {
    if (!str) return NULL;
    unsigned char uc = (unsigned char)c;
    char *cc = NULL;

    while (*str) {
        if ((unsigned char)(*str) == uc) cc = (char *)str;
        str++;
    }

    if (uc == '\0') return (char *)str;
    
    if (cc) return cc;
    return NULL;
}


char *strstr(const char *haystack, const char *needle) {
    if (!haystack || !needle) return NULL;

    if (*needle == '\0') return (char *)(haystack);

    size_t len_needle = strlen(needle);
    size_t len_haystack = strlen(haystack);
    for (size_t i = 0; i <= len_haystack - len_needle; ++i) {
        if (strncmp(haystack + i, needle, len_needle) == 0) return (char *)(haystack + i);
    }
    return NULL;
}


char *strtok(char *str, const char *delim) {
    if (!delim) return NULL;

    static char *next_token = NULL;

    if (str) {
        next_token = str;
    }

    if (next_token == NULL || *next_token == '\0') {
        return NULL;
    }


    while (*next_token) {
        int is_delim = 0;
        for (const char *d = delim; *d != '\0'; ++d) {
            if (*next_token == *d) {
                is_delim = 1;
                break;
            }
        }
        
        if (!is_delim) break;
        next_token++;
    }

    if (*next_token == '\0') {
        next_token = NULL;
        return NULL;
    }

    char *token_start = next_token;

    while (*next_token) {
        int is_delim = 0;

        for (const char *d = delim; *d != '\0'; ++d) {
            if (*next_token == *d) {
                is_delim = 1;
                break;
            }
        }

        if (is_delim) {
            *next_token = '\0';
            next_token++; 
            return token_start;
        }
        next_token++;
    }

    next_token = NULL;
    return token_start;
}