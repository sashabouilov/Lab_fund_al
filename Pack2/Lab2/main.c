#include "functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern void *memchr(const void *str, int c, size_t n);
extern int memcmp(const void *str1, const void *str2, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memset(void *str, int c, size_t n);
extern char *strncat(char *dest, const char *src, size_t n);
extern char *strchr(const char *str, int c);
extern int strncmp(const char *str1, const char *str2, size_t n);
extern char *strncpy(char *dest, const char *src, size_t n);
extern size_t strcspn(const char *str1, const char *str2);
extern char *strerror(int errnum);
extern size_t strlen(const char *str);
extern char *strpbrk(const char *str1, const char *str2);
extern char *strrchr(const char *str, int c);
extern char *strstr(const char *haystack, const char *needle);
extern char *strtok(char *str, const char *delim);

void print_usage() {
    printf("String Functions Testing Tool\n");
    printf("=============================\n");
    printf("Usage:\n");
    printf("  string_functions test <function_name> [args...]\n");
    printf("  string_functions demo\n");
    printf("\nAvailable functions:\n");
    printf("  memchr    - поиск символа в памяти\n");
    printf("  memcmp    - сравнение областей памяти\n");
    printf("  memcpy    - копирование памяти\n");
    printf("  memset    - заполнение памяти\n");
    printf("  strncat   - конкатенация строк\n");
    printf("  strchr    - поиск первого вхождения символа\n");
    printf("  strncmp   - сравнение строк\n");
    printf("  strncpy   - копирование строки\n");
    printf("  strcspn   - длина сегмента без указанных символов\n");
    printf("  strerror  - сообщение об ошибке\n");
    printf("  strlen    - длина строки\n");
    printf("  strpbrk   - поиск первого совпадающего символа\n");
    printf("  strrchr   - поиск последнего вхождения символа\n");
    printf("  strstr    - поиск подстроки\n");
    printf("  strtok    - разбиение строки на токены\n");
    printf("\nExamples:\n");
    printf("  string_functions test memchr \"Hello World\" W 11\n");
    printf("  string_functions test strlen \"Test String\"\n");
    printf("  string_functions test strchr \"Hello\" l\n");
    printf("  string_functions test strncmp \"abc\" \"abd\" 3\n");
    printf("  string_functions demo\n");
}

void print_status(void) {
    str_status_t status = get_last_error();
    const char *status_str;
    switch (status) {
        case STR_SUCCESS: status_str = "SUCCESS"; break;
        case STR_NULL_POINTER: status_str = "NULL_POINTER"; break;
        case STR_INVALID_SIZE: status_str = "INVALID_SIZE"; break;
        case STR_NOT_FOUND: status_str = "NOT_FOUND"; break;
        case STR_NO_MEMORY: status_str = "NO_MEMORY"; break;
        case STR_INVALID_PARAM: status_str = "INVALID_PARAM"; break;
        default: status_str = "UNKNOWN"; break;
    }
    printf("Status: %s\n", status_str);
}

void test_memchr(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: test memchr <string> <char> <size>\n");
        printf("Example: test memchr \"Hello World\" W 11\n");
        return;
    }
    
    char *str = argv[2];
    char c = argv[3][0];
    size_t n = (size_t)atoi(argv[4]);
    
    printf("Testing memchr:\n");
    printf("  String: \"%s\"\n", str);
    printf("  Character: '%c'\n", c);
    printf("  Search size: %zu\n", n);
    
    void *result = memchr(str, c, n);
    
    if (result != NULL) {
        printf("  Found at position: %lld\n", (long long)((char*)result - str));
        printf("  Remaining string: \"%s\"\n", (char*)result);
    } else {
        printf("  Character not found\n");
    }
    print_status();
}

void test_memcmp(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: test memcmp <str1> <str2> <size>\n");
        printf("Example: test memcmp \"abc\" \"abd\" 3\n");
        return;
    }
    
    char *str1 = argv[2];
    char *str2 = argv[3];
    size_t n = (size_t)atoi(argv[4]);
    
    printf("Testing memcmp:\n");
    printf("  String 1: \"%s\"\n", str1);
    printf("  String 2: \"%s\"\n", str2);
    printf("  Compare size: %zu\n", n);
    
    int result = memcmp(str1, str2, n);
    
    printf("  Result: %d\n", result);
    if (result < 0) {
        printf("  String 1 < String 2\n");
    } else if (result > 0) {
        printf("  String 1 > String 2\n");
    } else {
        printf("  Strings are equal\n");
    }
    print_status();
}

void test_memcpy(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: test memcpy <dest> <src> <size>\n");
        printf("Example: test memcpy \"initial\" \"copied\" 6\n");
        return;
    }
    
    char dest[100];
    char *src = argv[3];
    size_t n = (size_t)atoi(argv[4]);
    
    strncpy(dest, argv[2], sizeof(dest) - 1);
    dest[sizeof(dest) - 1] = '\0';
    
    printf("Testing memcpy:\n");
    printf("  Initial dest: \"%s\"\n", dest);
    printf("  Source: \"%s\"\n", src);
    printf("  Copy size: %zu\n", n);
    
    void *result = memcpy(dest, src, n);
    
    printf("  Result: \"%s\"\n", (char*)result);
    print_status();
}

void test_memset(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: test memset <string> <char> <size>\n");
        printf("Example: test memset \"Hello\" X 3\n");
        return;
    }
    
    char str[100];
    char c = argv[3][0];
    size_t n = (size_t)atoi(argv[4]);
    
    strncpy(str, argv[2], sizeof(str) - 1);
    str[sizeof(str) - 1] = '\0';
    
    printf("Testing memset:\n");
    printf("  Initial string: \"%s\"\n", str);
    printf("  Fill character: '%c'\n", c);
    printf("  Fill size: %zu\n", n);
    
    void *result = memset(str, c, n);
    
    printf("  Result: \"%s\"\n", (char*)result);
    print_status();
}

void test_strncat(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: test strncat <dest> <src> <size>\n");
        printf("Example: test strncat \"Hello\" \" World\" 6\n");
        return;
    }
    
    char dest[100];
    char *src = argv[3];
    size_t n = (size_t)atoi(argv[4]);
    
    strncpy(dest, argv[2], sizeof(dest) - 1);
    dest[sizeof(dest) - 1] = '\0';
    
    printf("Testing strncat:\n");
    printf("  Initial dest: \"%s\"\n", dest);
    printf("  Source: \"%s\"\n", src);
    printf("  Append size: %zu\n", n);
    
    char *result = strncat(dest, src, n);
    
    printf("  Result: \"%s\"\n", result);
    print_status();
}

void test_strchr(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: test strchr <string> <char>\n");
        printf("Example: test strchr \"Hello\" l\n");
        return;
    }
    
    char *str = argv[2];
    char c = argv[3][0];
    
    printf("Testing strchr:\n");
    printf("  String: \"%s\"\n", str);
    printf("  Character: '%c'\n", c);
    
    char *result = strchr(str, c);
    
    if (result != NULL) {
        printf("  Found at position: %lld\n", (long long)(result - str));
        printf("  Remaining string: \"%s\"\n", result);
    } else {
        printf("  Character not found\n");
    }
    print_status();
}

void test_strncmp(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: test strncmp <str1> <str2> <size>\n");
        printf("Example: test strncmp \"apple\" \"apples\" 5\n");
        return;
    }
    
    char *str1 = argv[2];
    char *str2 = argv[3];
    size_t n = (size_t)atoi(argv[4]);
    
    printf("Testing strncmp:\n");
    printf("  String 1: \"%s\"\n", str1);
    printf("  String 2: \"%s\"\n", str2);
    printf("  Compare size: %zu\n", n);
    
    int result = strncmp(str1, str2, n);
    
    printf("  Result: %d\n", result);
    if (result < 0) {
        printf("  String 1 < String 2\n");
    } else if (result > 0) {
        printf("  String 1 > String 2\n");
    } else {
        printf("  Strings are equal\n");
    }
    print_status();
}

void test_strncpy(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: test strncpy <dest> <src> <size>\n");
        printf("Example: test strncpy \"old\" \"new text\" 5\n");
        return;
    }
    
    char dest[100];
    char *src = argv[3];
    size_t n = (size_t)atoi(argv[4]);
    
    printf("Testing strncpy:\n");
    printf("  Source: \"%s\"\n", src);
    printf("  Copy size: %zu\n", n);
    
    char *result = strncpy(dest, src, n);
    dest[n] = '\0';
    
    printf("  Result: \"%s\"\n", result);
    print_status();
}

void test_strcspn(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: test strcspn <str1> <str2>\n");
        printf("Example: test strcspn \"Hello123\" \"0123456789\"\n");
        return;
    }
    
    char *str1 = argv[2];
    char *str2 = argv[3];
    
    printf("Testing strcspn:\n");
    printf("  String: \"%s\"\n", str1);
    printf("  Reject chars: \"%s\"\n", str2);
    
    size_t result = strcspn(str1, str2);
    
    printf("  Length of initial segment: %zu\n", result);
    printf("  Segment: \"");
    for (size_t i = 0; i < result && str1[i] != '\0'; i++) {
        printf("%c", str1[i]);
    }
    printf("\"\n");
    print_status();
}

void test_strerror(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: test strerror <errnum>\n");
        printf("Example: test strerror 2\n");
        printf("Error codes: 0-13 (0=Success, 2=File not found, 5=Access denied)\n");
        return;
    }
    
    int errnum = atoi(argv[2]);
    
    printf("Testing strerror:\n");
    printf("  Error number: %d\n", errnum);
    
    char *result = strerror(errnum);
    
    printf("  Error message: \"%s\"\n", result);
    print_status();
}

void test_strlen(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: test strlen <string>\n");
        printf("Example: test strlen \"Hello World\"\n");
        return;
    }
    
    char *str = argv[2];
    
    printf("Testing strlen:\n");
    printf("  String: \"%s\"\n", str);
    
    size_t result = strlen(str);
    
    printf("  Length: %zu\n", result);
    print_status();
}

void test_strpbrk(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: test strpbrk <str1> <str2>\n");
        printf("Example: test strpbrk \"Hello World\" \"aeiou\"\n");
        return;
    }
    
    char *str1 = argv[2];
    char *str2 = argv[3];
    
    printf("Testing strpbrk:\n");
    printf("  String: \"%s\"\n", str1);
    printf("  Search chars: \"%s\"\n", str2);
    
    char *result = strpbrk(str1, str2);
    
    if (result != NULL) {
        printf("  Found '%c' at position: %lld\n", *result, (long long)(result - str1));
        printf("  Remaining string: \"%s\"\n", result);
    } else {
        printf("  No matching characters found\n");
    }
    print_status();
}

void test_strrchr(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: test strrchr <string> <char>\n");
        printf("Example: test strrchr \"Hello\" l\n");
        return;
    }
    
    char *str = argv[2];
    char c = argv[3][0];
    
    printf("Testing strrchr:\n");
    printf("  String: \"%s\"\n", str);
    printf("  Character: '%c'\n", c);
    
    char *result = strrchr(str, c);
    
    if (result != NULL) {
        printf("  Last occurrence at position: %lld\n", (long long)(result - str));
        printf("  Remaining string: \"%s\"\n", result);
    } else {
        printf("  Character not found\n");
    }
    print_status();
}

void test_strstr(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: test strstr <haystack> <needle>\n");
        printf("Example: test strstr \"Hello World\" \"World\"\n");
        return;
    }
    
    char *haystack = argv[2];
    char *needle = argv[3];
    
    printf("Testing strstr:\n");
    printf("  Haystack: \"%s\"\n", haystack);
    printf("  Needle: \"%s\"\n", needle);
    
    char *result = strstr(haystack, needle);
    
    if (result != NULL) {
        printf("  Found at position: %lld\n", (long long)(result - haystack));
        printf("  Remaining string: \"%s\"\n", result);
    } else {
        printf("  Substring not found\n");
    }
    print_status();
}

void test_strtok(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: test strtok <string> <delim>\n");
        printf("Example: test strtok \"apple,banana,cherry\" \",\"\n");
        return;
    }
    
    char str[100];
    char *delim = argv[3];
    
    strncpy(str, argv[2], sizeof(str) - 1);
    str[sizeof(str) - 1] = '\0';
    
    printf("Testing strtok:\n");
    printf("  String: \"%s\"\n", str);
    printf("  Delimiters: \"%s\"\n", delim);
    
    char *token = strtok(str, delim);
    int token_count = 0;
    
    printf("  Tokens:\n");
    while (token != NULL) {
        printf("    [%d] \"%s\"\n", ++token_count, token);
        token = strtok(NULL, delim);
    }
    
    if (token_count == 0) {
        printf("    No tokens found\n");
    }
    
    print_status();
}

void run_demo() {
    printf("=== String Functions Demo ===\n\n");
    
    printf("1. Testing strlen:\n");
    size_t len = strlen("Hello, World!");
    printf("   strlen(\"Hello, World!\") = %zu\n", len);
    print_status();
    printf("\n");
    
    printf("2. Testing strncpy:\n");
    char dest[20];
    char *result = strncpy(dest, "Hello", 10);
    dest[10] = '\0';
    printf("   strncpy(dest, \"Hello\", 10) = \"%s\"\n", result);
    print_status();
    printf("\n");
    
    printf("3. Testing strncat:\n");
    char dest2[20] = "Hello";
    result = strncat(dest2, ", World!", 8);
    printf("   strncat(\"Hello\", \", World!\", 8) = \"%s\"\n", result);
    print_status();
    printf("\n");
    
    printf("4. Testing strchr:\n");
    char *chr_result = strchr("Hello, World!", 'W');
    printf("   strchr(\"Hello, World!\", 'W') = \"%s\"\n", chr_result);
    print_status();
    printf("\n");
    
    printf("5. Testing strstr:\n");
    char *str_result = strstr("This is a test string", "test");
    printf("   strstr(\"This is a test string\", \"test\") = \"%s\"\n", str_result);
    print_status();
    printf("\n");
    
    printf("6. Testing strerror:\n");
    for (int i = 0; i <= 5; i++) {
        char *err_msg = strerror(i);
        printf("   strerror(%d) = \"%s\"\n", i, err_msg);
    }
    print_status();
    printf("\n");
    
    printf("7. Testing strtok:\n");
    char str[] = "apple,banana,cherry,date";
    printf("   Original: \"%s\"\n", str);
    char *token = strtok(str, ",");
    printf("   Tokens: ");
    while (token != NULL) {
        printf("\"%s\" ", token);
        token = strtok(NULL, ",");
    }
    printf("\n");
    print_status();
    printf("\n");
    
    printf("8. Testing memchr:\n");
    char buffer[] = "Search in this text";
    void *mem_result = memchr(buffer, 'i', 15);
    if (mem_result != NULL) {
        printf("   memchr(\"Search in this text\", 'i', 15) = found at position %lld\n", 
               (long long)((char*)mem_result - buffer));
    }
    print_status();
    printf("\n");
    
    printf("9. Testing memcmp:\n");
    int cmp_result = memcmp("abc", "abd", 3);
    printf("   memcmp(\"abc\", \"abd\", 3) = %d\n", cmp_result);
    print_status();
    printf("\n");
    
    printf("10. Testing strcspn:\n");
    size_t cspn_result = strcspn("Hello123", "0123456789");
    printf("   strcspn(\"Hello123\", \"0123456789\") = %zu\n", cspn_result);
    print_status();
    printf("\n");
    
    printf("Demo completed!\n");
}

int main(int argc, char *argv[]) {
    printf("String Functions Testing Tool\n");
    printf("\n");
    
    if (argc < 2) {
        print_usage();
        return 1;
    }
    
    if (strcmp(argv[1], "demo") == 0) {
        run_demo();
        return 0;
    }
    
    if (strcmp(argv[1], "test") != 0 || argc < 3) {
        print_usage();
        return 1;
    }
    
    char *function_name = argv[2];
    
    if (strcmp(function_name, "memchr") == 0) {
        test_memchr(argc, argv);
    } else if (strcmp(function_name, "memcmp") == 0) {
        test_memcmp(argc, argv);
    } else if (strcmp(function_name, "memcpy") == 0) {
        test_memcpy(argc, argv);
    } else if (strcmp(function_name, "memset") == 0) {
        test_memset(argc, argv);
    } else if (strcmp(function_name, "strncat") == 0) {
        test_strncat(argc, argv);
    } else if (strcmp(function_name, "strchr") == 0) {
        test_strchr(argc, argv);
    } else if (strcmp(function_name, "strncmp") == 0) {
        test_strncmp(argc, argv);
    } else if (strcmp(function_name, "strncpy") == 0) {
        test_strncpy(argc, argv);
    } else if (strcmp(function_name, "strcspn") == 0) {
        test_strcspn(argc, argv);
    } else if (strcmp(function_name, "strerror") == 0) {
        test_strerror(argc, argv);
    } else if (strcmp(function_name, "strlen") == 0) {
        test_strlen(argc, argv);
    } else if (strcmp(function_name, "strpbrk") == 0) {
        test_strpbrk(argc, argv);
    } else if (strcmp(function_name, "strrchr") == 0) {
        test_strrchr(argc, argv);
    } else if (strcmp(function_name, "strstr") == 0) {
        test_strstr(argc, argv);
    } else if (strcmp(function_name, "strtok") == 0) {
        test_strtok(argc, argv);
    } else {
        printf("Unknown function: %s\n\n", function_name);
        print_usage();
        return 1;
    }
    
    return 0;
}