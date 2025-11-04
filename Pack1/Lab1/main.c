#include "head.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <number> <flag>\n", argv[0]);
        printf("Available flags: -h, -p, -s, -e, -a, -f\n");
        return 1;
    }

    for (int i = 0; argv[1][i] != '\0'; i++) {
        if (!isdigit(argv[1][i]) && argv[1][i] != '-') {
            printf("Error: First argument must be a number\n");
            return 1;
        }
    }

    int x = atoi(argv[1]);
    char *flag = argv[2];
    char simb = flag[0];

    if(x < 0){
        printf("Number less than zero\n");
        return 1;
    }

    if(strcmp(flag, "-h") == 0 || strcmp(flag, "/h") == 0){
        h_flag(x);
    } else if(strcmp(flag, "-p") == 0 || strcmp(flag, "/p") == 0){
        p_flag(x);
    } else if(strcmp(flag, "-s") == 0 || strcmp(flag, "/s") == 0){
        s_flag(x);
    } else if(strcmp(flag, "-e") == 0 || strcmp(flag, "/e") == 0){
        e_flag(x);
    } else if(strcmp(flag, "-a") == 0 || strcmp(flag, "/a") == 0){
        a_flag(x);
    } else if(strcmp(flag, "-f") == 0 || strcmp(flag, "/f") == 0){
        f_flag(x);
    } else {
        printf("You need to enter the data in the following format: number -flag(/flag)\n");
        return 1;
    }
    return 0;
}