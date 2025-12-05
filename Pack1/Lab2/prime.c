#include "prime.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

static status_t sieve_of_eratosthenes(uint64_t limit, uint64_t** primes_ptr, uint32_t* count_ptr) {
    if (limit < 2) {
        *count_ptr = 0;
        *primes_ptr = NULL;
        return SUCCESS;
    }

    bool* is_prime = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_prime) {
        return MEMORY_ERROR;
    }

    for (uint64_t i = 2; i <= limit; i++) {
        is_prime[i] = true;
    }

    for (uint64_t i = 2; i * i <= limit; i++) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    uint32_t prime_count = 0;
    for (uint64_t i = 2; i <= limit; i++) {
        if (is_prime[i]) {
            prime_count++;
        }
    }

    uint64_t* primes = (uint64_t*)malloc(prime_count * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return MEMORY_ERROR;
    }

    uint32_t index = 0;
    for (uint64_t i = 2; i <= limit; i++) {
        if (is_prime[i]) {
            primes[index++] = i;
        }
    }

    free(is_prime);
    *primes_ptr = primes;
    *count_ptr = prime_count;
    
    return SUCCESS;
}

status_t find_nth_prime(uint32_t n, uint64_t* result) {
    if (n == 0) {
        return INVALID_INPUT;
    }
    
    if (result == NULL) {
        return INVALID_INPUT;
    }

    uint64_t limit;
    if (n < 6) {
        limit = 12;
    } else {
        double log_n = log((double)n);
        double log_log_n = log(log_n);
        limit = (uint64_t)(n * (log_n + log_log_n));
        if (limit < n) {
            limit = n * 2;
        }
    }

    while (1) {
        uint64_t* primes = NULL;
        uint32_t prime_count = 0;
        status_t status = sieve_of_eratosthenes(limit, &primes, &prime_count);
        
        if (status != SUCCESS) {
            return status;
        }
        
        if (n <= prime_count) {
            *result = primes[n - 1];
            free(primes);
            return SUCCESS;
        }
        
        free(primes);
        limit *= 2;
        
        if (limit < 1000000) {
            limit *= 2;
        } else {
            limit += limit / 2;
        }
    }
}

status_t validate_input(int t, const int* queries, int query_count) {
    if (t <= 0 || t != query_count) {
        return INVALID_INPUT;
    }
    
    for (int i = 0; i < query_count; i++) {
        if (queries[i] <= 0) {
            return INVALID_INPUT;
        }
    }
    
    return SUCCESS;
}


void print_results(const uint64_t* results, const int* queries, int count, const status_t* statuses) {
    for (int i = 0; i < count; i++) {
        switch (statuses[i]) {
            case SUCCESS:
                printf("%llu\n", (unsigned long long)results[i]);
                break;
            case INVALID_INPUT:
                printf("Invalid request: %d\n", queries[i]);
                break;
            case MEMORY_ERROR:
                printf("Memory error processing request: %d\n", queries[i]);
                break;
            default:
                printf("Unknown error for request: %d\n", queries[i]);
                break;
        }
    }
}