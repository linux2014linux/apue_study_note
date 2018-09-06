#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int toi(const char str[]) {
    if (NULL == str) {
        return 0;
    }

    size_t i = 0;
    int sum = 0;
    for (i = strlen(str) - 1; i > 0; i--) {
        if (int(str[i]) < int('0') || int(str[i]) > int('9')) {
            printf("%c is invalid.", str[i]);
            return 0;
        }
        int cur = (int(str[i]) - int('0')) * (int)pow(10, strlen(str) - i - 1);
        sum += cur;
    }

    if (str[i] == '-') {
        sum = ~sum;
    } else if (str[i] == '+') {
        ;
    } else if (int(str[i]) >= int('0') || int(str[i]) <= int('9')) {
        sum += (int(str[i]) - int('0')) * (int)pow(10, strlen(str) - i - 1);
    } else {
        sum = 0;
    }

    return sum;
}

int main(int argc, char* argv[]) {

    printf("ret=%d\n", toi(argv[1]));

    return 0;
}
