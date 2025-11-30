#include <stdio.h>
#include "functions.h"

int main(void) {
    const char data[] = "Hello, world!";
    void *p = memchr(data, 'w', sizeof(data));
    if (p) {
        printf("memchr: the character 'w' was found at %p\n", p);
    } else {
        printf("memchr: the symbol was not found\n");
    }

    const char a[] = "abc";
    const char b[] = "abd";
    int cmp = memcmp(a, b, 3);
    printf("memcmp(\"abc\", \"abd\", 3) = %d\n", cmp);

    char dest[20];
    memcpy(dest, "Hello, world!", 8);
    printf("memcpy: %s\n", dest);

    memset(dest, '-', 5);
    printf("memset: %s\n", dest);

    char buf[50] = "Hello";
    strncat(buf, " World", 6);
    printf("strncat: %s\n", buf);

    char *pos = strchr("GoodBye, World!", ',');
    if (pos) {
        printf("strchr: first ',' found, the remainder of the string: %s\n", pos);
    }

    int scmp = strncmp("apple", "apples", 5);
    printf("strncmp(\"apple\", \"apples\", 5) = %d\n", scmp);

    char dst[10];
    strncpy(dst, "123456789", 9);
    dst[9] = '\0';
    printf("strncpy: %s\n", dst);

    size_t span = strcspn("hello123", "0123456789");
    printf("strcspn: length up to a digit = %zu\n", span);

    size_t len = strlen("example");
    printf("strlen(\"example\") = %zu\n", len);

    char *brk = strpbrk("hello world", "owr");
    if (brk) {
        printf("strpbrk: the first matching character -> '%c'\n", *brk);
    }

    char *str = "banana";
    char *last = strrchr(str, 'a');
    if (last) {
        printf("strrchr: last 'a' -> position %ld\n", last - str);
    }

    char *found = strstr("hello goodbye privet poka", "privet");
    if (found) {
        printf("strstr: %s\n", found);
    }

    char text[] = "red,green;blue";
    char *token = strtok(text, ",;");
    printf("strtok: tokens -> ");
    while (token) {
        printf("[%s] ", token);
        token = strtok(NULL, ",;");
    }
    printf("\n");

    return 0;
}