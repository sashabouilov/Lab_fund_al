#include "prime.h"
#include <stdio.h>
#include <stdlib.h>


int main(){
    int t;

    printf("Enter the number of requests: ");
    
    if (scanf("%d", &t) != 1) {
        printf("Input error\n");
        return 1;
    }

    int* numbers = (int*)malloc(t * sizeof(int));
    if (!numbers) {
        printf("Memory allocation error\n");
        return 1;
    }

    printf("Enter the ordinal numbers of the primes:\n");

    for (int i = 0; i < t; i++) {
        if (scanf("%d", &numbers[i]) != 1) {
            printf("Input error\n");
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

    printf("Output:\n");

    for (int i = 0; i < t; i++) {
        uint64_t prime;
        status_t status = find_nth_prime(numbers[i], &prime);
        
        switch (status) {
            case SUCCESS:
                printf("%lu\n", prime);
                break;
            case INVALID_INPUT:
                printf("Invalid request: %d\n", numbers[i]);
                break;
            case MEMORY_ERROR:
                printf("Memory error\n");
                free(numbers);
                return 1;
            default:
                printf("Unknown error\n");
                free(numbers);
                return 1;
        }
    }

    free(numbers);
    return 0;
}