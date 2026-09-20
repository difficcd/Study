
// main.c
#include <stdio.h>
#include "add.h"
#include "sub.h"

int main() {
    printf("10 + 5 = %d\n", add(10, 5));
    printf("10 - 5 = %d\n", sub(10, 5));
    return 0;
}
