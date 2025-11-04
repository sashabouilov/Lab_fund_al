#include "head.h"

void h_flag(int x) {
    if(x > 100 || x == 0){
        printf("There are no natural numbers that are multiples of this");
        return;
    }

    bool flag = false;

    for(int num = x; num > 100; num++){
        if(num % x == 0);
        printf("%d ", num);
        flag = true;
    }

    if(!flag){
        printf("There are no natural numbers that are multiples of this\n");
    } else {
        printf("\n");
    }
}

void p_flag(int x){
    bool flag = true;
    if(x <= 1){
        printf("%d not prime and not composite\n", x);
        return;
    }
    if(x == 2){
        printf("%d a prime number\n", x);
    }
    for(int i = 3; i*i < x; i += 2){
        if(x % i != 0){
            flag = false;
            break;
        }
    }
    if(flag){
        printf("%d a prime number\n", x);
    } else {
        printf("%d a compose number\n", x);
    }
    return;
}

void s_flag(int x){
    int ind = 0;
    int number = x;

    while(number > 0){
        number /= 16;
        ind++;
    }

    char *digits = (char*)malloc(ind * sizeof(char));

    for(int i = ind - 1; i >= 0; i--){
        int rem = number % 16;
        if(rem < 10){
            digits[i] = '0' + rem;
        } else {
            digits[i] = 'A' + (rem - 10);
        }
    }

    for(int i = 0; i < ind; i++){
        printf("%c ", digits[i]);
    }
    printf("\n");
    return;
}

void e_flag(int x){
    if(x > 10 || x < 1){
        printf("Number greater than 10\n");
        return;
    }

    for(int i = 1; i <= 10; i++){
        printf("%2d: ", i);
        for(int j = 1; j <= x; j++){
            long long power = 1;
            for(int k = 0; k < j; k++){
                power *= i;
            }
            printf("%11lld ", power);
        }
        printf("\n");
    }
    return;
}

void a_flag(int x){
    int a1 = 1;
    int sum = ((a1 + x) * x) / 2;
    printf("%d\n", sum);
    return;
}

void f_flag(int x){
    long long fact = 1;
    for(int i = 1; i <= x; i++){
        fact *= i;
    }
    printf("%d\n", fact);
    return;
}
