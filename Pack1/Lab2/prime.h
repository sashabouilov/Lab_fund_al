#ifndef PRIME_FINDER_H
#define PRIME_FINDER_H

#include <stdint.h>
#include <math.h>

typedef enum {
    SUCCESS = 0,
    INVALID_INPUT = 1,
    MEMORY_ERROR = 2
} status_t;

status_t find_nth_prime(uint32_t n, uint64_t* result);
status_t validate_input(int t, int* queries, int query_count);

#endif