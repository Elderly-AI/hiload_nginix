#include <stdio.h>
#include <unistd.h>
#include "hello.h"

void hello(void *args) {
    int *num = args;
    sleep(1);
    printf("HELLO WORLD  %d\n", *num);
}
