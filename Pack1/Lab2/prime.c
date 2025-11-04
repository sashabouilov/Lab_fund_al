#include "prime.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

static status_t eratosthen(uint64_t limit, uint64_t** primes, uint32_t* count) {

    if (limit < 2) {
        *count = 0;
        return SUCCESS;
    }

    bool* is_prime = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_prime) return MEMORY_ERROR;

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
        if (is_prime[i]) prime_count++;
    }

    *primes = (uint64_t*)malloc(prime_count * sizeof(uint64_t));

    if (!*primes) {
        free(is_prime);
        return MEMORY_ERROR;
    }

    uint32_t index = 0;

    for (uint64_t i = 2; i <= limit; i++) {
        if (is_prime[i]) {
            (*primes)[index++] = i;
        }
    }

    *count = prime_count;
    free(is_prime);
    return SUCCESS;
}

status_t find_nth_prime(int n, uint64_t* result) {

    if (n == 0){
      return INVALID_INPUT;  
    } 
    
    uint64_t limit = (n < 6) ? 12 : (uint64_t)(n * (log(n) + log(log(n))));
    
    uint64_t* primes = NULL;
    uint32_t prime_count = 0;
    status_t status = eratosthen(limit, &primes, &prime_count);
    
    if (status != SUCCESS){
       return status; 
    }
    
    if (n <= prime_count) {
        *result = primes[n - 1];
        free(primes);
        return SUCCESS;
    }
    
    free(primes);
    limit *= 2;
    return find_nth_prime(n, result);
}

status_t validate_input(int t, int* queries, int query_count) {

    if (t <= 0 || t != query_count) return INVALID_INPUT;
    
    for (int i = 0; i < query_count; i++) {
        if (queries[i] <= 0){
          return INVALID_INPUT;  
        } 
    }
    
    return SUCCESS;
}
