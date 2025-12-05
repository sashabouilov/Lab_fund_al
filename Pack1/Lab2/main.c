#include "prime.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int t;
    
    printf("Enter the number of requests: ");
    
    if (scanf("%d", &t) != 1) {
        printf("Input error: cannot read number of requests\n");
        return 1;
    }
    
    if (t <= 0) {
        printf("Error: number of requests must be positive\n");
        return 1;
    }
    
    int* numbers = (int*)malloc((size_t)t * sizeof(int));
    if (!numbers) {
        printf("Memory allocation error\n");
        return 1;
    }
    
    printf("Enter the ordinal numbers of the primes:\n");
    
    for (int i = 0; i < t; i++) {
        if (scanf("%d", &numbers[i]) != 1) {
            printf("Input error at request %d\n", i + 1);
            free(numbers);
            return 1;
        }
    }
    
    status_t validation_status = validate_input(t, numbers, t);
    
    if (validation_status != SUCCESS) {
        printf("Incorrect input data\n");
        free(numbers);
        return 1;
    }
    
    uint64_t* results = (uint64_t*)malloc((size_t)t * sizeof(uint64_t));
    status_t* statuses = (status_t*)malloc((size_t)t * sizeof(status_t));
    
    if (!results || !statuses) {
        printf("Memory allocation error\n");
        free(numbers);
        if (results) free(results);
        if (statuses) free(statuses);
        return 1;
    }
    
    printf("Output:\n");
    
    for (int i = 0; i < t; i++) {
        statuses[i] = find_nth_prime((uint32_t)numbers[i], &results[i]);
    }
    
    print_results(results, numbers, t, statuses);
    
    free(numbers);
    free(results);
    free(statuses);
    
    return 0;
}